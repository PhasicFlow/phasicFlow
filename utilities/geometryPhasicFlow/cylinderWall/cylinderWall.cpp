#include "cylinderWall.hpp"
#include "line.hpp"


bool pFlow::cylinderWall::readCylinderWall(const dictionary& dict)
{
	auto p1 = dict.getVal<realx3>("p1");
	auto p2 = dict.getVal<realx3>("p2");
	auto radius1 = dict.getVal<real>("radius1");
	auto radius2 = dict.getVal<real>("radius2") ;

	int32 resolution = dict.getValOrSet("resolution", 24 );
	int32 zResolution = dict.getValOrSet("zResolution", 1);

	
	triangles_.clear();
	triangles_.reserve(2*resolution*zResolution);


	line cylAxis(p1, p2);
	auto lp1 = p1;

	auto dt = static_cast<real>(1.0/zResolution);
	real t = 0;
	for(int32 i=0; i<zResolution; i++)
	{
		t += dt;
		auto lp2 = cylAxis.point(t);
		if(!createCylinder(lp1, lp2, radius1, radius2, resolution))
			return false;

		lp1 = lp2;
	}

	return true;
}

bool pFlow::cylinderWall::createCylinder(
	const realx3& p1,
	const realx3& p2,
	real rad1,
	real rad2,
	int32 numDiv)
{

	zAxis zAx(p1, p2);
	
	real L = zAx.length();

	
	realx3Vector r1P(numDiv + 1), r2P(numDiv + 1);
	real dTheta = 2 * Pi / numDiv;
	real theta = 0;

	for (int32 i = 0; i < numDiv + 1; i++)
	{
		r1P[i] = realx3(rad1*cos(theta), rad1*sin(theta), 0);
		r2P[i] = realx3(rad2*cos(theta), rad2*sin(theta), L);
		theta += dTheta;
	}
	
	// transferring back all points to the original axis of cylinder
	for (int32 i = 0; i < numDiv + 1; i++)
	{
		r1P[i] = zAx.transferBackZ(r1P[i]);
		r2P[i] = zAx.transferBackZ(r2P[i]);
	}
	
	realx3 norm;
	for (int32 i = 0; i < numDiv; i++)
	{
		realx3 p1 = r1P[i];
		realx3 p2 = r2P[i];
		realx3 p3 = r2P[i + 1];
		realx3 p4 = r1P[i + 1];

		if(checkNormalVec(p1, p2, p3, norm))
		{
			triangles_.push_back(realx3x3(p1, p2, p3));
		}
		else
		{
			fatalErrorInFunction<<
			"planner input triangle: "<<realx3x3(p1, p2, p3)<<endl;
			return false;
		}
		
		if (checkNormalVec(p3, p4, p1, norm))
		{
			triangles_.push_back(realx3x3(p3, p4, p1));
		}
		else
		{
			fatalErrorInFunction<<
			"planner input triangle: "<<realx3x3(p3, p4, p1)<<endl;
			return false;
		}

	}

	return true;
}


pFlow::cylinderWall::cylinderWall()
{}


pFlow::cylinderWall::cylinderWall(
	const dictionary& dict
)
:
	Wall(dict)
{
	if( !readCylinderWall(dict) )
	{
		fatalExit;
	}
}
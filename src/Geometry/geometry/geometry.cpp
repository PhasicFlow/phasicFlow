/*------------------------------- phasicFlow ---------------------------------
      O        C enter of
     O O       E ngineering and
    O   O      M ultiscale modeling of
   OOOOOOO     F luid flow       
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------  
Licence:
  This file is part of phasicFlow code. It is a free software for simulating 
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions. 
 
  phasicFlow is distributed to help others in their research in the field of 
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/

#include "geometry.hpp"
#include "systemControl.hpp"
#include "vocabs.hpp"


bool pFlow::geometry::createPropertyId()
{
	if(materialName_.size() != numSurfaces() )
	{
		fatalErrorInFunction<<
		"number of subSurface and material names do not match"<<endl;
		return false;
	}

	uint32Vector propId(
		"propId", 
		capacity(), 
		size(),
		RESERVE());

	ForAll(i, materialName_)
	{
		uint32 pIdx =0;
		
		if( !wallProperty_.nameToIndex(materialName_[i], pIdx) )
		{
			fatalErrorInFunction<<
			"Property/material name is invalid "<<materialName_[i]<<
			". A list of valid names are \n"<< wallProperty_.materials()<<endl;
			return false;
		}
		
		auto triRange = subSurfaceRange(i);
		propId.fill(triRange.start(), triRange.end(), pIdx);
		
	}

	propertyId_.assign(propId);

	return true;

}

void pFlow::geometry::zeroForce()
{
	contactForceWall_.fill(zero3);
}

pFlow::geometry::geometry
(
	systemControl& control,
	const property& prop
)
:
	multiTriSurface
	(
		objectFile
		(
			triSurfaceFile__,
			"",
			objectFile::READ_ALWAYS,
			objectFile::WRITE_ALWAYS
		),
		&control.geometry()
	),
	demComponent
	(
		"geometry",
		control
	),
	wallProperty_(prop),
	propertyId_
	(
		objectFile
		(
			"propertyId",
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_NEVER
		),
		*this,
		0u
	),
	contactForceWall_
	(
		objectFile
		(
			"contactForcWall",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_NEVER
		),
		*this,
		zero3
	),
	normalStressWall_
	(
		objectFile
		(
			"normalStressWall",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_NEVER
		),
		*this,
		zero3
	),
	shearStressWall_
	(
		objectFile
		(
			"shearStressWall",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_NEVER
		),
		*this,
		zero3
	)
{

	readWholeObject_ = false;
	if( !IOobject::readObject() )
    {
        fatalErrorInFunction<<
        "Error in reading from file "<<IOobject::path()<<endl;
        fatalExit;
    }
	readWholeObject_ = true;

	if( this->numSurfaces() != motionComponentName_.size() )
	{
		fatalErrorInFunction<<
		"Number of surfaces is not equal to number of motion component names"<<endl;
		fatalExit;
	}
	
	if(!createPropertyId())
	{
		fatalExit;
	}
}

pFlow::geometry::geometry
(
	systemControl &control, 
	const property &prop, 
	multiTriSurface &surf, 
	const wordVector &motionCompName, 
	const wordVector &materialName,
	const dictionary& motionDict
)
:
	multiTriSurface
	(
		objectFile
		(
			triSurfaceFile__,
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_ALWAYS
		),
		&control.geometry(),
		surf
	),
	demComponent
	(
		"geometry",
		control
	),
	wallProperty_
	(
		prop
	),
	propertyId_
	(
		objectFile
		(
			"propertyId",
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_NEVER
		),
		*this,
		0u
	),
	contactForceWall_
	(
		objectFile
		(
			"contactForcWall",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_NEVER
		),
		*this,
		zero3
	),
	normalStressWall_
	(
		objectFile
		(
			"normalStressWall",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_NEVER
		),
		*this,
		zero3
	),
	shearStressWall_
	(
		objectFile
		(
			"shearStressWall",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_NEVER
		),
		*this,
		zero3
	)
	
{
	motionComponentName_.assign(motionCompName);
	materialName_.assign(materialName);


	if( this->numSurfaces() != motionComponentName_.size() )
	{
		fatalErrorInFunction<<
		"Number of surfaces ("<< this->numSurfaces() <<
        ") is not equal to number of motion component names("<<
        motionComponentName_.size()<<")"<<endl;
		fatalExit;
	}

	if(!createPropertyId())
	{
		fatalExit;
	}
}

bool pFlow::geometry::beforeIteration()
{
	zeroForce();
    return true;
}

bool pFlow::geometry::iterate()
{
    return true;
}

bool pFlow::geometry::afterIteration()
{
	auto numTris = size();
	auto& normalsD = normals().deviceViewAll();
	auto& areaD = area().deviceViewAll();
	auto& cForceD = contactForceWall_.deviceViewAll();
	auto& sStressD = shearStressWall_.deviceViewAll();
	auto& nStressD = normalStressWall_.deviceViewAll();

	Kokkos::parallel_for(
		"pFlow::geometry::afterIteration",
		deviceRPolicyStatic(0, numTris),
		LAMBDA_HD(uint32 i)
		{	
			realx3 n = normalsD[i];
			real A = max(areaD[i],smallValue);
			realx3 nF = dot(cForceD[i],n)*n;
			realx3 sF = cForceD[i] - nF;
			nStressD[i] = nF/A;
			sStressD[i] = sF/A;
		});
	Kokkos::fence();
    
	return true;
}

bool pFlow::geometry::read(iIstream &is, const IOPattern &iop)
{
	motionComponentName_.read(is, iop);
	
	materialName_.read(is, iop);
	
	if( readWholeObject_ )
	{
		return multiTriSurface::read(is, iop);
	}

    return true;
}

bool pFlow::geometry::write(iOstream &os, const IOPattern &iop) const
{
    
	if( !motionComponentName_.write(os, iop) )
	{
		fatalErrorInFunction;
		return false;
	}

	if( !materialName_.write(os,iop))
	{
		fatalErrorInFunction;
		return false;
	}
	
	return multiTriSurface::write(os,iop);
}


pFlow::uniquePtr<pFlow::geometry> 
	pFlow::geometry::create
(
	systemControl& control,
	const property& prop
)
{
	
	//
	fileDictionary dict( motionModelFile__, control.time().geometry().path());

	word model = dict.getVal<word>("motionModel");
	
	auto geomModel = angleBracketsNames("geometry", model);

	REPORT(1)<< "Selecting geometry model . . ."<<END_REPORT;
	if( systemControlvCtorSelector_.search(geomModel) )
	{
		auto objPtr = systemControlvCtorSelector_[geomModel] (control, prop);
		REPORT(2)<<"Model "<< Green_Text(geomModel)<<" is created.\n"<<END_REPORT;
		return objPtr;
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< Yellow_Text(geomModel) << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			systemControlvCtorSelector_
		);
		fatalExit;
	}

	return nullptr;
}

pFlow::uniquePtr<pFlow::geometry> 
	pFlow::geometry::create
	(
		systemControl& control, 
		const property& prop,
		multiTriSurface& surf,
		const wordVector& motionCompName,
		const wordVector& materialName,
		const dictionary& motionDic
	)
{

	word model = motionDic.getVal<word>("motionModel");

	auto geomModel = angleBracketsNames("geometry", model);

	REPORT(1)<< "Selecting geometry model . . ."<<END_REPORT;

	if( dictionaryvCtorSelector_.search(geomModel) )
	{
		auto objPtr = dictionaryvCtorSelector_[geomModel] 
			(
				control,
				prop, 
				surf,
				motionCompName,
				materialName,
				motionDic
			);
		REPORT(2)<<"Model "<< Green_Text(geomModel)<<" is created.\n"<<END_REPORT;
		return objPtr;
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< Yellow_Text(geomModel) << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			dictionaryvCtorSelector_
		);
		fatalExit;
	}
	return nullptr;
}




/*pFlow::geometry::geometry
(
	systemControl& control,
	const property& prop,
	const multiTriSurface& triSurface,
	const wordVector& motionCompName,
	const wordVector& matName
)
:
	demGeometry(control),
	wallProperty_(prop),
	geometryRepository_(control.geometry()),
	triSurface_(
		control.geometry().emplaceObject<multiTriSurface>(
			objectFile(
				"triSurface",
				"",
				objectFile::READ_NEVER,
				objectFile::WRITE_ALWAYS
				),
			triSurface
			)
		),
	motionComponentName_(
		control.geometry().emplaceObject<wordField>(
			objectFile(
				"motionComponentName",
				"",
				objectFile::READ_NEVER,
				objectFile::WRITE_ALWAYS	
				),
			"motionNamesList",
			motionCompName
			)
		),
	materialName_(
		control.geometry().emplaceObject<wordField>(
			objectFile(
				"materialName",
				"",
				objectFile::READ_NEVER,
				objectFile::WRITE_ALWAYS
				),
			"materialNamesList",
			matName
			)
		),
	propertyId_(
		control.geometry().emplaceObject<int8TriSurfaceField_D>(
			objectFile(
				"propertyId",
				"",
				objectFile::READ_NEVER,
				objectFile::WRITE_NEVER),
			surface(),
			0 ) ),
	contactForceWall_(
		control.geometry().emplaceObject<realx3TriSurfaceField_D>(
			objectFile(
				"contactForceWall",
				"",
				objectFile::READ_NEVER,
				objectFile::WRITE_ALWAYS),
			surface(),
			zero3) ),
	stressWall_(
		control.geometry().emplaceObject<realx3TriSurfaceField_D>(
			objectFile(
				"stressWall",
				"",
				objectFile::READ_NEVER,
				objectFile::WRITE_ALWAYS),
			surface(),
			zero3) )
{
	if(!findPropertyId())
	{
		fatalExit;
	}
}

pFlow::geometry::geometry
(
	systemControl& control,
	const property& prop,
	const dictionary& dict,
	const multiTriSurface& triSurface,
	const wordVector& motionCompName,
	const wordVector& matName
)
:
	geometry(control, prop, triSurface, motionCompName, matName)
{}




bool pFlow::geometry::beforeIteration()
{ 
	this->zeroForce();
	return true;	
}


bool pFlow::geometry::afterIteration()
{ 
	
	auto Force = contactForceWall_.deviceVectorAll();
	auto area = triSurface_.area().deviceVectorAll();
	auto stress = stressWall_.deviceVectorAll();
	auto numTri =triSurface_.size();
	

	Kokkos::parallel_for(
		"geometry::calculateStress",
		numTri,
		LAMBDA_HD(int32 i){
			stress[i] = Force[i]/area[i];
		});
	Kokkos::fence();
	return true;
	
}*/

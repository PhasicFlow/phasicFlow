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

#include "geometry.H"
#include "vocabs.H"


bool pFlow::geometry::findPropertyId()
{

	int8Vector propId(0, surface().capacity(),RESERVE());
	propId.clear();

	uint32 pId;
	forAll(matI, materialName_)
	{
		
		if( !wallProperty_.nameToIndex( materialName_[matI], pId ) )
		{
		 	fatalErrorInFunction<<
		 	"material name for the geometry is invalid: "<< materialName_[matI]<<endl;
		 	return false;
		}

		int32 surfSize = surface().surfNumTriangles(matI);

		for(int32 i=0; i<surfSize; i++)
		{
			propId.push_back(pId);
		}
	}	

	propertyId_.assign(propId);

	return true;

}

pFlow::geometry::geometry
(
	systemControl& control,
	const property& prop
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
				objectFile::READ_ALWAYS,
				objectFile::WRITE_ALWAYS
				)
			)
		),
	motionComponentName_(
		control.geometry().emplaceObject<wordField>(
			objectFile(
				"motionComponentName",
				"",
				objectFile::READ_ALWAYS,
				objectFile::WRITE_ALWAYS	
				),
			"motionNamesList"
			)
		),
	materialName_(
		control.geometry().emplaceObject<wordField>(
			objectFile(
				"materialName",
				"",
				objectFile::READ_ALWAYS,
				objectFile::WRITE_ALWAYS
				),
			"materialNamesList"
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
				objectFile::READ_IF_PRESENT,
				objectFile::WRITE_ALWAYS),
			surface(),
			zero3) ),
	stressWall_(
		control.geometry().emplaceObject<realx3TriSurfaceField_D>(
			objectFile(
				"stressWall",
				"",
				objectFile::READ_IF_PRESENT,
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
				objectFile::READ_IF_PRESENT,
				objectFile::WRITE_ALWAYS),
			surface(),
			zero3) ),
	stressWall_(
		control.geometry().emplaceObject<realx3TriSurfaceField_D>(
			objectFile(
				"stressWall",
				"",
				objectFile::READ_IF_PRESENT,
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


pFlow::uniquePtr<pFlow::geometry> 
	pFlow::geometry::create
(
	systemControl& control,
	const property& prop
)
{
	//motionModelFile__
	auto motionDictPtr = IOobject::make<dictionary>
	(
		objectFile
		(
			motionModelFile__,
			control.geometry().path(),
			objectFile::READ_ALWAYS,
			objectFile::WRITE_NEVER
		),
		motionModelFile__,
		true
	);

	word model = motionDictPtr().getObject<dictionary>().getVal<word>("motionModel");
	
	auto geomModel = angleBracketsNames("geometry", model);

	Report(1)<< "Selecting geometry model . . ."<<endReport;
	if( systemControlvCtorSelector_.search(geomModel) )
	{
		auto objPtr = systemControlvCtorSelector_[geomModel] (control, prop);
		Report(2)<<"Model "<< greenText(geomModel)<<" is created.\n"<<endReport;
		return objPtr;
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< yellowText(geomModel) << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			systemControlvCtorSelector_
		);
		fatalExit;
	}

	return nullptr;
}

pFlow::uniquePtr<pFlow::geometry> 
	pFlow::geometry::create(
			systemControl& control,
			const property& prop,
			const dictionary& dict,
			const multiTriSurface& triSurface,
			const wordVector& motionCompName,
			const wordVector& propName)
{

	word model = dict.getVal<word>("motionModel");

	auto geomModel = angleBracketsNames("geometry", model);

	Report(1)<< "Selecting geometry model . . ."<<endReport;

	if( dictionaryvCtorSelector_.search(geomModel) )
	{
		auto objPtr = dictionaryvCtorSelector_[geomModel] 
			(
				control,
				prop, 
				dict,
				triSurface,
				motionCompName,
				propName
			);
		Report(2)<<"Model "<< greenText(geomModel)<<" is created.\n"<<endReport;
		return objPtr;
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< yellowText(geomModel) << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			dictionaryvCtorSelector_
		);
		fatalExit;
	}
	return nullptr;
}
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


#include "positionParticles.hpp"
#include "pointStructure.hpp"
#include "setFields.hpp"
#include "systemControl.hpp"
#include "commandLine.hpp"
#include "vocabs.hpp"
#include "baseShapeNames.hpp"

//#include "readControlDict.hpp"

using namespace pFlow;

int main( int argc, char* argv[] )
{

	commandLine cmds(
		"createParticles",
		"Read the dictionary createParticles and create particles"
		" based on the two sub-dictionaries positionParticles and setFields.\n"
		"First executes positionParticles and then setFields, except "
		"otherwise selected in the command line.");

			
	bool positionOnly = false;
	cmds.add_flag(
		"--positionParticles-only",
		positionOnly,
		"Exectue the positionParticles part only and store the created "
		"pointStructure in the time folder.");
	
	bool setOnly = false;
	cmds.add_flag("--setFields-only",
		setOnly,
		"Exectue the setFields part only. Read the pointStructure from "
		"time folder and setFields and save the result in the same time folder.");

	bool isCoupling = false;
	cmds.add_flag(
		"-c,--coupling",
		isCoupling,
		"Is this a fluid-particle coupling simulation");

	if(!cmds.parse(argc, argv)) return 0;
	
	if(setOnly && positionOnly)
	{
		ERR<<
		"Options --positionParticles-only and --setFields-only cannot be used simeltanuously. \n"<<END_ERR;
		return 1;
	}	

// this should be palced in each main 
#include "initialize_Control.hpp"

	fileDictionary cpDict("particlesDict", Control.settings().path());
	

	uniquePtr<pointStructure> pStructPtr = nullptr;


	if(!setOnly)
	{

		// position particles based on the dict content 
		REPORT(0)<< "Positioning points . . . \n"<<END_REPORT;
		auto pointPosition = positionParticles::create(Control, cpDict.subDict("positionParticles"));

		fileSystem pStructPath = Control.time().path()+pointStructureFile__;

		auto finalPos = pointPosition().getFinalPosition();

		pStructPtr = makeUnique<pointStructure>(Control, finalPos);


		REPORT(1)<< "Created pStruct with "<< pStructPtr().size() << " points and capacity "<<
		pStructPtr().capacity()<<" . . ."<< END_REPORT;

	}
    else
	{
		// read the content of pStruct from 0/pStructure
		pStructPtr = makeUnique<pointStructure>(Control);

	}

	List<uniquePtr<IOobject>> allObjects;

	if(!positionOnly)
	{
        
		auto& pStruct = pStructPtr();

		auto& sfDict = cpDict.subDict("setFields");

		setFieldList defValueList(sfDict.subDict("defaultValue"));

		for(auto& sfEntry: defValueList)
		{
			if( auto Ptr = sfEntry.setPointFieldDefaultValueNewAll( pStruct, true); Ptr)
			{
				allObjects.push_back(std::move(Ptr));
			}
			else
			{
				ERR<< "\n error occured in setting default value fields.\n"<<END_ERR;
				return 1;
			}
		}
		
		output<<endl;

		auto& selectorsDict = sfDict.subDict("selectors");

		auto selNames = selectorsDict.dictionaryKeywords();

		for(auto name: selNames)
		{
			REPORT(1)<< "Applying selector " << Green_Text(name) <<END_REPORT;
			
			if(
				!applySelector(Control, pStruct, selectorsDict.subDict(name))
			 )
			{
				ERR<<"\n error occured in setting selector. \n"<<END_ERR;
				return 1;
			}
			output<<endl;
		}
	}

	Control.clearIncludeExclude();
	Control.addExclude("shapeName");

	uint32PointField_H shapeIndex
	(
		objectFile
		(
			"shapeIndex",
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_ALWAYS
		),
		pStructPtr(),
		0u
	);

	baseShapeNames shapes(
		shapeFile__,
		&Control.caseSetup()
	);
	
	auto& shapeName = Control.time().template lookupObject<wordPointField_H>("shapeName");

	REPORT(0)<< "Converting shapeName field to shapeIndex field"<<END_REPORT;

	auto shapeName_D = shapeName.fieldDevice();
	auto shapeIndex_D = shapeIndex.fieldDevice();
	
	REPORT(1)<<"List of shape names in "<<shapes.globalName()<<
	" is: "<<Green_Text(shapes.shapeNameList())<<END_REPORT;

	ForAll(i, shapeIndex)
	{
		if(uint32 index; shapes.shapeNameToIndex(shapeName_D[i], index))
		{
			shapeIndex_D[i] = index;
		}
		else
		{
			fatalErrorInFunction<<"Found shape name "<< Yellow_Text(shapeName_D[i])<<
			"in shapeName field. But the list of shape names in file "<< 
			shapes.globalName()<<" is : \n"<<
			shapes.shapeNames()<<endl;
		}
	}

    if( !Control.time().write(true))
    {
        fatalErrorInFunction<<
        "ERRor in writing to file. \n ";
        return 1; 
    }
	REPORT(0)<< Green_Text("\nFinished successfully.\n")<<END_REPORT;

	
// this should be palced in each main 
#include "finalize.hpp"

	return 0;
} 

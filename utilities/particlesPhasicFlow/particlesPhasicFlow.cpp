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

//#include "readControlDict.hpp"

using pFlow::output;
using pFlow::endl;
using pFlow::dictionary;
using pFlow::uniquePtr;
using pFlow::IOobject;
using pFlow::objectFile;
using pFlow::fileSystem;
using pFlow::pointStructure;
using pFlow::pointStructureFile__;
using pFlow::positionParticles;
using pFlow::commandLine;
using pFlow::setFieldList;

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

	auto objCPDict = IOobject::make<dictionary>
	(
		objectFile
		(
			"particlesDict",
			Control.settings().path(),
			objectFile::READ_ALWAYS,
			objectFile::WRITE_ALWAYS
		),
		"particlesDict",
		true
	);

	auto& cpDict = objCPDict().getObject<dictionary>();

	pointStructure* pStructPtr = nullptr;


	if(!setOnly)
	{

		// position particles based on the dict content 
		REPORT(0)<< "Positioning points . . . \n"<<END_REPORT;
		auto pointPosition = positionParticles::create(Control, cpDict.subDict("positionParticles"));

		fileSystem pStructPath = Control.time().path()+pointStructureFile__;

		auto finalPos = pointPosition().getFinalPosition();

		
		auto& pStruct = Control.time().emplaceObject<pointStructure>
		(
			objectFile
			(
				pointStructureFile__,
				Control.time().path(),
				objectFile::READ_NEVER,
				objectFile::WRITE_ALWAYS			
			),
			Control,
            finalPos
		);

		pStructPtr = &pStruct;


		REPORT(1)<< "Created pStruct with "<< pStruct.size() << " points and capacity "<<
		pStruct.capacity()<<" . . ."<< END_REPORT;

		REPORT(1)<< "Writing pStruct to " << Control.time().path()+ pointStructureFile__<< END_REPORT<<endl<<endl;

		if( !Control.time().write())
		{
			fatalErrorInFunction<<
			"ERRor in writing to file. \n ";
			return 1; 
		}
	}else
	{

		auto& pStruct = Control.time().emplaceObject<pointStructure>
		(
			objectFile
			(
				pointStructureFile__,
				Control.time().path(),
				objectFile::READ_NEVER,
				objectFile::WRITE_ALWAYS			
			),
            Control
		);

		pStructPtr = &pStruct;

	}

	

	if(!positionOnly)
	{

		auto& pStruct = *pStructPtr;

		auto& sfDict = cpDict.subDict("setFields");

		setFieldList defValueList(sfDict.subDict("defaultValue"));

		for(auto& sfEntry: defValueList)
		{
			if( !sfEntry.setPointFieldDefaultValueNewAll(Control.time(), pStruct, true))
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

	Control.time().write(true);
	REPORT(0)<< Green_Text("\nFinished successfully.\n")<<END_REPORT;


// this should be palced in each main 
#include "finalize.hpp"

	return 0;
} 



/*
uniquePtr<IOobject> pStructObj{nullptr};

	if(!setOnly)
	{

		// position particles based on the dict content 
		REPORT(0)<< "Positioning points . . . \n"<<endREPORT;
		auto pointPosition = positionParticles::create(cpDict.subDict("positionParticles"));

		fileSystem pStructPath = Control.time().path()+pointStructureFile__;

		auto finalPos = pointPosition().getFinalPosition();

		
		pStructObj = IOobject::make<pointStructure>
		(
			objectFile
			(
				pointStructureFile__,
				Control.time().path(),
				objectFile::READ_NEVER,
				objectFile::WRITE_ALWAYS			
			),
			finalPos
		);

		auto& pSruct = pStructObj().getObject<pointStructure>();

		REPORT(1)<< "Created pStruct with "<< pSruct.size() << " points and capacity "<<
		pSruct.capacity()<<" . . ."<< endREPORT;

		REPORT(1)<< "Writing pStruct to " << pStructObj().path() << endREPORT<<endl<<endl;

		if( !pStructObj().write())
		{
			fatalErrorInFunction<<
			"ERRor in writing to file. \n ";
			return 1; 
		}
	}else
	{
		pStructObj = IOobject::make<pointStructure>
		(
			objectFile
			(
				pointStructureFile__,
				Control.time().path(),
				objectFile::READ_ALWAYS,
				objectFile::WRITE_NEVER			
			)
		);
	}

	

	if(!positionOnly)
	{

		auto& pStruct = pStructObj().getObject<pointStructure>();

		auto& sfDict = cpDict.subDict("setFields");

		setFieldList defValueList(sfDict.subDict("defaultValue"));

		for(auto& sfEntry: defValueList)
		{
			if( !sfEntry.setPointFieldDefaultValueNewAll(Control.time(), pStruct, true))
			{
				ERR<< "\n error occured in setting default value fields.\n"<<endERR;
				return 1;
			}
		}
		
		output<<endl;

		auto& selectorsDict = sfDict.subDict("selectors");

		auto selNames = selectorsDict.dictionaryKeywords();

		for(auto name: selNames)
		{
			REPORT(1)<< "Applying selector " << greenText(name) <<endREPORT;
			
			if(
				!applySelector(Control, pStruct, selectorsDict.subDict(name))
			 )
			{
				ERR<<"\n error occured in setting selector. \n"<<endERR;
				return 1;
			}
			output<<endl;
		}
	}
	*/
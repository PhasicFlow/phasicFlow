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

#include "postprocess.hpp" 
#include "timeFolder.hpp"
#include "pointStructure.hpp"
#include "vocabs.hpp"
#include "vtkFile.hpp"

pFlow::postprocess::postprocess(systemControl& control)
:
	control_(control),
	dict_(postprocessFile__, control_.settings().path()+postprocessFile__)
{
	Report(1)<<"Reading numberBased dictionary ..."<<endReport;
	auto nbDict = dict_.subDictOrCreate("numberBased");

	numberBasedDictNames_ = dict_.subDictOrCreate("numberBased").dictionaryKeywords();
	if(!numberBasedDictNames_.empty())
	{
		Report(1)<< "numberBased dictionary contains " << yellowText(numberBasedDictNames_)<<endReport<<endl;	
	}
	

	weightBasedDictNames_ = dict_.subDictOrCreate("weightBased").dictionaryKeywords();
	if(!weightBasedDictNames_.empty())
	{
		Report(1)<< "numberBased dictionary contains " << yellowText(weightBasedDictNames_)<<endReport<<endl;	
	}
	

}



bool pFlow::postprocess::processTimeFolder(real time, const word& tName, const fileSystem& localFolder)
{
	
	time_ = time;

	Report(0)<<"Working on time folder "<< cyanText(time)<<endReport; 
	timeFolderReposiory_ = 
		makeUnique<repository>
		(
			"timeFolder-"+tName,
			localFolder,
			&control_
		);
	
	Report(1)<<"Reading pointStructure"<<endReport;
	timeFolderReposiory().emplaceObject<pointStructure>(
		objectFile
		(
			pFlow::pointStructureFile__,
			"",
			objectFile::READ_ALWAYS,
			objectFile::WRITE_NEVER
		));	

	
	Report(1)<<"Creating mesh and point to cell mapper"<<endReport;
	pointToCell_ = makeUnique<pointRectCell>(
		dict_.subDict("rectMesh"),
		timeFolderReposiory().lookupObject<pointStructure>(pointStructureFile__),
		timeFolderReposiory());

	// first numberbased dict 
	processedFields_.clear();
	for(word& dictName:numberBasedDictNames_)
	{
		
		
		auto fieldDict  = dict_.subDict("numberBased").subDict(dictName);
		auto ppFieldPtr = processField::create(
			fieldDict,
			pointToCell_(),
			timeFolderReposiory());

		if(!ppFieldPtr->process())
		{
			fatalExit;
		}

		processedFields_.push_back( ppFieldPtr.release() );

		output<<endl;

	}



	return true;
}


bool pFlow::postprocess::processTimeFolder(const timeFolder& tFolder)
{
	return processTimeFolder(
		tFolder.time(),
		tFolder.timeName(),
		tFolder.localFolder() );
}


bool pFlow::postprocess::writeToVTK(fileSystem destPath, word bName)const
{
	vtkFile vtk(destPath, bName, time_);

	if(!vtk) return false;

	Report(1)<<"Writing processed fields to vtk file..."<<endReport;
	// mesh
	pointToCell_->mesh().writeToVtk(vtk());

	forAll( i, processedFields_)
	{
		
		if( !processedFields_[i].writeToVTK(vtk()))
		{

			fatalErrorInFunction<<"error in writing "<< processedFields_[i].processedFieldName()<<
			"to vtk file\n";
			return false;
		}
	}

	return true;
}

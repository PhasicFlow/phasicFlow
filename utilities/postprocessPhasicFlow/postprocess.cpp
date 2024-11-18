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
#include "countFields.hpp"
#include "vocabs.hpp"
#include "vtkFile.hpp"

pFlow::postprocess::postprocess(systemControl& control)
:
	control_(control),
	dict_(postprocessFile__, control_.settings().path()+postprocessFile__)
{
	REPORT(1)<<"Reading numberBased dictionary ..."<<END_REPORT;
	auto nbDict = dict_.subDictOrCreate("numberBased");

	numberBasedDictNames_ = dict_.subDictOrCreate("numberBased").dictionaryKeywords();
	if(!numberBasedDictNames_.empty())
	{
		REPORT(1)<< "numberBased dictionary contains " << Yellow_Text(numberBasedDictNames_)<<END_REPORT<<endl;	
	}
	
	weightBasedDictNames_ = dict_.subDictOrCreate("weightBased").dictionaryKeywords();
	if(!weightBasedDictNames_.empty())
	{
		REPORT(1)<< "numberBased dictionary contains " << Yellow_Text(weightBasedDictNames_)<<END_REPORT<<endl;	
	}

	countDictNames_ = dict_.subDictOrCreate("counting").dictionaryKeywords();
	if(!countDictNames_.empty())
	{
		REPORT(1)<< "counting dictionary contains " << Yellow_Text(countDictNames_)<<END_REPORT<<endl;	
	}

}



bool pFlow::postprocess::processTimeFolder(real time, const word& tName, const fileSystem& localFolder)
{
	
	time_ = time;

	REPORT(0)<<"Working on time folder "<< Cyan_Text(time)<<END_REPORT; 
	
	control_.time().setTime(time);

	REPORT(1)<<"Reading pointStructure"<<END_REPORT;
	pointStructure pStruct(control_);

	// first delete the object to remove fields from repository 
	pointToCell_.reset(nullptr);
	
	REPORT(1)<<"Creating mesh and point to cell mapper"<<END_REPORT;
	pointToCell_ = makeUnique<pointRectCell>(
		dict_.subDict("rectMesh"),
		pStruct,
		control_.time());

	// first numberbased dict 
	
	processedFields_.clear();
	for(word& dictName:numberBasedDictNames_)
	{
		auto fieldDict  = dict_.subDict("numberBased").subDict(dictName);
		auto ppFieldPtr = processField::create(
			fieldDict,
			pointToCell_(),
			control_.time());

		if(!ppFieldPtr->process())
		{
			fatalExit;
		}

		processedFields_.push_back( ppFieldPtr.release() );
		output<<endl;
	}

	countedVariableNamesList_.clear();
	countedVairablesLists_.clear();

	for(const auto& countDictName:countDictNames_)
	{
		REPORT(1)<<"Processing "<< Yellow_Text("counting."<<countDictName)<<END_REPORT;
		const dictionary& countDict = dict_.subDict("counting").subDict(countDictName);

		countFields cFields(countDict);

		cFields.process(timeFolderReposiory());

		countedVariableNamesList_.push_back(
			cFields.variableNames());

		countedVairablesLists_.push_back(cFields.countedValues());

	}
	output<<"\n";


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

	REPORT(1)<<"Writing processed fields to vtk file..."<<END_REPORT;
	// mesh
	pointToCell_->mesh().writeToVtk(vtk());

	ForAll( i, processedFields_)
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

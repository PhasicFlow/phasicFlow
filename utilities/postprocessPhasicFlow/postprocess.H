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

#ifndef __postprocess_H__ 
#define __postprocess_H__


#include "MapPtr.H"
#include "systemControl.H"
#include "pointRectCell.H"
#include "processField.H"
	

namespace pFlow
{

class timeFolder;


class postprocess
{
protected:

	systemControl& 			control_;

	dictionary 				dict_;

	wordList 				numberBasedDictNames_;

	wordList 				weightBasedDictNames_;

	uniquePtr<repository> 	timeFolderReposiory_ {nullptr};

	uniquePtr<pointRectCell>	pointToCell_ {nullptr};

	//uniquePtr<repository> 	processedRepository_ {nullptr};

	ListPtr<processField> 	processedFields_;

	real 					time_=0.0;

	//orderedMapPtr<real, repository> 	timeFolderRepositories_;


	Logical 	saveTimes{"No"};

	Logical		saveTimeFolders{"No"};


	auto& timeFolderReposiory()
	{
		return timeFolderReposiory_();
	}

public:

	postprocess(systemControl& control);



	bool processTimeFolder(real time, const word& tName, const fileSystem& localFolder);

	bool processTimeFolder(const timeFolder& tFolder);


	bool writeToVTK(fileSystem path, word bName)const;


};

	

} 



#endif //__postprocess_H__

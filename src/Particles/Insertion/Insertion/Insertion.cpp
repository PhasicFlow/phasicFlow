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



template<typename ShapeType>
bool pFlow::Insertion<ShapeType>::writeInsertionDict
(
	dictionary& dict
)const
{
	
	if(!insertion::writeInsertionDict(dict))return false;

	if( !this->isActive() ) return true;

	ForAll(i,regions_)
	{
		auto& rgnDict = dict.subDictOrCreate(regions_[i].name());

		if( !regions_[i].write(rgnDict) )
		{
			fatalErrorInFunction<<
			"Error in writing to dictionary "<<rgnDict.globalName()<<endl;
			return false;
		}
	}

	return true;
}

template<typename ShapeType>
bool
pFlow::Insertion<ShapeType>::readInsertionDict()
{
	regions_.clear();
	
	if( !this->isActive() )
	{
		return true;
	}

	wordList regionDicNames = this->dictionaryKeywords();

	for(const auto& name:regionDicNames)
	{
		REPORT(2)<<"reading insertion region "<< Green_Text(name)<<END_REPORT;
		
		regions_.push_back(makeUnique<InsertionRegion<ShapeType>>(
			name,
			*this, 
			shapes_));
	}

	return true;

}

template<typename ShapeType>
pFlow::Insertion<ShapeType>::Insertion(
	particles& prtcl,
	const ShapeType& shapes)
:
	insertion(prtcl),
	shapes_(shapes)
{
	if(!readInsertionDict())
	{
		fatalErrorInFunction;
		fatalExit;
	}	
}

/*template<typename ShapeType>
pFlow::Insertion<ShapeType>::Insertion(
	fileSystem file, 
	particles& prtcl, 
	const ShapeType& shapes)
:
	Insertion(prtcl, shapes)
{
	dictionary inDict(file.fileName(), file);

	if(!readInsertionDict(inDict))
	{
		fatalErrorInFunction<< "could not read from file "<<
		file<<endl;
		fatalExit;
	}
}*/


template<typename ShapeType>
bool pFlow::Insertion<ShapeType>::insertParticles
(
	uint32 iter, 
	real t,
	real dt
)
{
	if(!isActive()) return true;
	

	ForAll(i,regions_)
	{
		bool insertionOccured = false;
		auto& rgn = regions_[i];
		if( rgn.insertionTime(iter, t, dt) )
		{
			
			realx3Vector pos;
			wordVector shapes;
			if( rgn.insertParticles(iter, t, dt, shapes, pos, insertionOccured) )
			{

				if(insertionOccured)
				{
					REPORT(0)<<"\nParticle insertion from "<< Green_Text(rgn.name())<<END_REPORT;
					REPORT(1)<< Cyan_Text(pos.size()) << " new particles is being inserted at Time: "<<
						Cyan_Text(t) <<" s."<<END_REPORT;
						
					if(!Particles().insertParticles(pos, shapes, rgn.setFieldList()))
					{
						fatalErrorInFunction<<
						" Cannot add "<< pos.size() << " particles from region "<< rgn.name() << 
						" to particles. \n";
						return false;
					}
					REPORT(1)<<"Total number of particles inserted from this region is "<<
						Cyan_Text(rgn.totalInserted())<<'\n'<<END_REPORT;
				}
				else
				{
					continue;
				}
				
			}
			else
			{
				if(insertionOccured)
				{
					WARNING<< "\n fewer number of particles are inserted from region "<< rgn.name() <<
					" than expected. You may stop the simulation to change settings."<<END_WARNING;
					if(!Particles().insertParticles(pos, shapes, rgn.setFieldList()))
					{
						fatalErrorInFunction<<
						" Cannot add "<< pos.size() << " particles from region "<< rgn.name() << 
						" to particles. \n";
						return false;
					}
				}
				else
				{
					fatalErrorInFunction<<
					"  error in inserting particles from region "<< rgn.name()<<endl;
					return false;	
				}
				
			}
		}
		
	}

	return true;
}


/*template<typename ShapeType>
bool pFlow::Insertion<ShapeType>::read
(
	iIstream& is
)
{

	// create an empty dictionary
	dictionary dict(is.name(), true);

	if(!dict.read(is))
	{
		ioErrorInFile( is.name(), is.lineNumber() )<<
		"  error in reading "<< insertionFile__ << "dictionary from file."<<endl;
		return false;
	}

	if(!readInsertionDict(dict))
	{
		fatalErrorInFunction<<
		"  error in reading from dictionary "<<dict.globalName()<<endl;
		return false;
	}
	
	return true;
}

template<typename ShapeType>
bool pFlow::Insertion<ShapeType>::write
(
	iOstream& os
)const
{

	dictionary dict(insertionFile__,true);

	if(! writeInsertionDict(dict) )
	{
		fatalErrorInFunction<<
		"  error in writing to " << dict.globalName()<<endl;
		return false;
	}

	if( !dict.write(os) )
	{
		ioErrorInFile(os.name(), os.lineNumber())<<
		"  erro in writing to "<< os.name()<<endl;
		return false;
	}	

	return true;
}*/
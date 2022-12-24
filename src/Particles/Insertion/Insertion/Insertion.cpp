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
bool pFlow::Insertion<ShapeType>::readInsertionDict
(
	const dictionary& dict
)
{
	if(!insertion::readInsertionDict(dict)) return false;
	
	regions_.clear();
	
	if( !this->isActive() )
	{
		return true;
	}

	wordList regionDicNames = dict.dictionaryKeywords();

	for(auto& name:regionDicNames)
	{
		REPORT(2)<<"reading insertion region "<< greenText(name)<<endREPORT;
		regions_.push_backSafe(dict.subDict(name), shapes_);
	}

	return true;
}

template<typename ShapeType>
bool pFlow::Insertion<ShapeType>::writeInsertionDict
(
	dictionary& dict
)const
{
	if( !insertion::writeInsertionDict(dict) ) return false;

	if( !this->isActive() ) return true;

	ForAll(i,regions_)
	{
		auto& rgnDict = dict.subDictOrCreate(regions_[i].name());

		if( !regions_[i].write(rgnDict) )
		{
			return false;
		}
	}

	return true;
}

template<typename ShapeType>
pFlow::Insertion<ShapeType>::Insertion
(
	particles& prtcl,
	const ShapeType& shapes
)
:
	insertion(prtcl),
	shapes_(shapes)
{

	
}


template<typename ShapeType>
bool pFlow::Insertion<ShapeType>::insertParticles
(
	real currentTime,
	real dt
)
{
	if(!isActive()) return true;
	

	ForAll(i,regions_)
	{
		bool insertionOccured = false;
		auto& rgn = regions_[i];
		if( rgn.insertionTime(currentTime, dt) )
		{
			
			realx3Vector pos;
			wordVector shapes;
			if( rgn.insertParticles(currentTime, dt, shapes, pos, insertionOccured) )
			{

				if(insertionOccured)
				{
					REPORT(0)<<"\nParticle insertion from "<< greenText(rgn.name())<<endREPORT;
					REPORT(1)<< cyanText(pos.size()) << " new particles is being inserted at Time: "<<
						cyanText(currentTime) <<" s."<<endREPORT;
						
					if(!particles_.insertParticles(pos, shapes, rgn.setFields()))
					{
						fatalErrorInFunction<<
						" Cannot add "<< pos.size() << " particles from region "<< rgn.name() << 
						" to particles. \n";
						return false;
					}
					REPORT(1)<<"Total number of particles inserted from this region is "<<
						cyanText(rgn.totalInserted())<<'\n'<<endREPORT;
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
					yWARNING<< "\n fewer number of particles are inserted from region "<< rgn.name() <<
					" than expected. You may stop the simulation to change settings."<<endyWARNING;
					continue;
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


template<typename ShapeType>
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
}
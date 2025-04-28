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


#include "stlFile.hpp"
#include "iFstream.hpp"
#include "oFstream.hpp"
#include "error.hpp"

namespace pFlow
{
inline bool badInput(iIstream& is, token& tok )
{
	return is.bad() || !tok.good();
}

inline bool checkWordToken(iIstream& is, token& tok, const word& check)
{

	if( badInput(is, tok) || is.eof() || !tok.isWord() || tok.wordToken() != check ) return false;
	return true;

}

inline bool checkNumberToken(iIstream& is, token& tok, real& val)
{
	if(badInput(is, tok) || is.eof() || !tok.isNumber() )return false;
	val = tok.number();
	return true;
}

}

bool pFlow::stlFile::readSolid
(
	iIstream& is,
	realx3x3Vector & vertecies,
	word & name
)
{

	token tok;
	is >> tok;
	if(!checkWordToken(is, tok, "solid")) return false;

	// check if there is a name associated with solid 
	name = "";
	
	
	int32 nWords =0;
	bool reachedFacet = false;
	is >> tok;

	while (nWords < 20 )
	{
		if( badInput(is, tok) ) return false;
		nWords++;	
		if(tok.isWord() && tok.wordToken() != "facet" )	
		{
			name += tok.wordToken();	
		}
		else if( tok.isNumber())
		{
			auto val = tok.number();
			name += real2Word(val);
		}
		else if( tok.isPunctuation())
		{
			name += tok.pToken();
		}
		else if (tok.isWord() && tok.wordToken() == "facet")
		{
			is.putBack(tok);
			reachedFacet = true;
			break;	
		}
		else
		{
			return false;
		}
		is >> tok;
	}

	if(!reachedFacet) return false;
		
	vertecies.clear();
	while(true )
	{
		is >> tok;
		if( badInput(is,tok) || !tok.isWord() )return false;
		word wTok = tok.wordToken();
		if( wTok == "endsolid" )// end of solid
		{
			// check if there is a name associated with endsolid
			is >> tok;
			if( !badInput(is, tok) && !is.eof())
			{
				word endName = "";
				int32 nWords =0;

				while (nWords < 20 )
				{
					if( badInput(is, tok) ) return false;
					nWords++;	
					if(tok.isWord())	
					{
						endName += tok.wordToken();	
					}
					else if( tok.isNumber())
					{
						auto val = tok.number();
						endName += real2Word(val);
					}
					else if( tok.isPunctuation())
					{
						endName += tok.pToken();
					}
					else if (tok.isWord())
					{
						is.putBack(tok);
						break;
					}
					else
					{
						return false;
					}
					is >> tok;

					if(is.eof())return true;
				}
			}
			return true;
		}

		if( wTok != "facet" ) return false;
		
		// read facet 
		is.putBack(tok);
		realx3x3 tri;
		if( !readFacet(is, tri) ) return false;

		vertecies.push_back(tri);
		
	}

	return true;	
}


bool pFlow::stlFile::readFacet
(
	iIstream& is,
	realx3x3& tri
)
{
	token tok;

	is>>tok;
	if( !checkWordToken(is, tok, "facet") ) return false;
	
	is >> tok;
	if( !checkWordToken(is, tok , "normal") ) return false;

	real val;
	for(uint32 i=0; i<3; i++ )
	{
		is>>tok;
		if( !checkNumberToken(is, tok, val))return false;
	}
	
	is>> tok;
	if( !checkWordToken(is, tok, "outer")) return false;

	is>> tok;
	if(!checkWordToken(is, tok, "loop")) return false;

	realx3 v;

	for(uint32 i=0; i<3; i++)
	{
		is>>tok;
		if(!checkWordToken(is, tok, "vertex")) return false;
		is>>tok;
		if(!checkNumberToken(is, tok, v.x()))return false;
		is>>tok;
		if(!checkNumberToken(is, tok, v.y()))return false;
		is>>tok;
		if(!checkNumberToken(is, tok, v.z()))return false;
		if( i==0 ) tri.x() = v;
		if( i==1 ) tri.y() = v;
		if( i==2)  tri.z() = v;
	}
	is>> tok;
	if(!checkWordToken(is, tok, "endloop")) return false;
	is>> tok;
	if(!checkWordToken(is, tok, "endfacet")) return false;

	return true;		
}

bool pFlow::stlFile::writeFacet
(
	iOstream& os,
	const realx3x3& tri
)const
{
	realx3 n = cross( tri.y() - tri.x(), tri.z()-tri.x());
	n.normalize();
	os.incrIndent();
	indent(os) << "facet" << spaceToken() 
			   << "normal"<<spaceToken() 
			   << n.x() << spaceToken() 
			   << n.y() << spaceToken()
			   << n.z() << endl;
	os.incrIndent();
	indent(os) << "outer loop"<<endl; 
	os.incrIndent();
	indent(os) << "vertex"<< spaceToken() 
			   << tri.x().x() << spaceToken() 
			   << tri.x().y() << spaceToken()
			   << tri.x().z() << endl;

	indent(os) << "vertex"<< spaceToken() 
			   << tri.y().x() << spaceToken() 
			   << tri.y().y() << spaceToken()
			   << tri.y().z() << endl;

	indent(os) << "vertex"<< spaceToken() 
			   << tri.z().x() << spaceToken() 
			   << tri.z().y() << spaceToken()
			   << tri.z().z() << endl;
	os.decrIndent();
	indent(os) << "endloop"<<endl;

	os.decrIndent();
	indent(os)<< "endfacet"<<endl;

	os.decrIndent();

	return true;

}


bool pFlow::stlFile::writeSolid
(
	iOstream& os,
	const realx3x3Vector& vertecies,
	const word& name
)const
{
	os<< "solid"<<spaceToken()<<name<<endl;
	for(const auto& tri: vertecies)
	{
		writeFacet(os, tri);
	}
	os<< "endsolid"<<endl;

	return true;
}

pFlow::stlFile::stlFile( fileSystem file )
:
	file_(file)
{
}

pFlow::stlFile::stlFile
(
	fileSystem file,
	const word& name,
	const realx3x3Vector& vertecies
)
:
	stlFile(file)
{
	addSolid(name, vertecies);
}


pFlow::stlFile::stlFile
(
	fileSystem file,
	const word& name,
	realx3x3Vector&& vertecies
)
:
	stlFile(file)
{
	addSolid(name, vertecies);
}

void pFlow::stlFile::addSolid
(
	const word& name,
	const realx3x3Vector& vertecies
)
{
	solids_.push_back(makeUnique<realx3x3Vector>(vertecies));
	solidNames_.push_back(name);
}


void pFlow::stlFile::addSolid
(
	const word& name,
	realx3x3Vector&& vertecies
)
{
	solids_.push_back(makeUnique<realx3x3Vector>(vertecies));
	solidNames_.push_back(name);
}



bool pFlow::stlFile::read()
{
	solids_.clear();
	solidNames_.clear();

	// open file
	iFstream is(file_);

	token tok;
	while (true)
	{
		
		realx3x3Vector vertecies;
		word name;
		if(!readSolid(is, vertecies, name))
		{
			ioErrorInFile(is.name(), is.lineNumber());
		 	return false;
		}
		
		addSolid(name, std::move(vertecies));
	
		is >> tok;
		if( is.eof() || !tok.good())return true;
		is.putBack(tok);
		
	}

	return true;
}


bool pFlow::stlFile::write()const
{
	oFstream os(file_);
	os.precision(8);
	for(size_t i=0; i<size(); i++)
	{
		writeSolid(os, solids_[i], solidNames_[i]);
	}

	return true;
}

void pFlow::stlFile::setFile
(
	fileSystem file
) const
{
	file_ = file;
}

const pFlow::wordList& pFlow::stlFile::names()const
{
	return solidNames_;
}

size_t pFlow::stlFile::size()const
{
	return solids_.size();
}

const pFlow::realx3x3Vector& pFlow::stlFile::solid
(
	size_t i
)const
{
	if(i >= size() )
	{
		fatalErrorInFunction<<
		"requested out of range solid from stlFile "<< 
		file_<<endl;
		fatalExit;
	}

	return solids_[i];
}

const pFlow::word& pFlow::stlFile::name
(
	size_t i
)const
{
	if(i >= size() )
	{
		fatalErrorInFunction<<
		"requested out of range solid name from stlFile "<< 
		file_<<endl;
		fatalExit;
	}

	return solidNames_[i];
}
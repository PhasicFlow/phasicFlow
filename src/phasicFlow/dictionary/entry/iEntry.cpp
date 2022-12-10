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
// based on OpenFOAM dictionary, with some modifications/simplifications
// to be tailored to our needs

#include "iEntry.hpp"
#include "dictionary.hpp"
#include "dataEntry.hpp"
#include "error.hpp"
#include "iOstream.hpp"
#include "iIstream.hpp"


bool pFlow::iEntry::readKeyword
(
	iIstream &is,
	word& keyword,
	token& tok
)
{
	
	while
	(
		!is.read(tok).bad() &&
		!is.eof() &&
		tok.good()
	)
	{
		if(tok == token::END_STATEMENT) continue;
		
		if( tok.isWord())
		{
			keyword = tok.wordToken();
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}


bool pFlow::iEntry::createEntry
(
	dictionary& parDict,
	iIstream& is,
	bool hasBlockToken
)
{
	is.fatalCheck(FUNCTION_NAME);

	// reads tokens one-by-one
    word key;
    token nextTok;
    
    if( auto isKey = iEntry::readKeyword(is, key, nextTok ); isKey )
    {
	    //output<< key << " "<<nextTok <<endl;
    	is.read(nextTok);
    	if( is.eof() || is.bad() || !nextTok.good() )
    	{
    		ioErrorInFile(is.name(), is.lineNumber())<<
    		"expecting a valid token after keyword " << key << endl;
    		fatalExit;
    	}
       // output<< nextTok<<endl;
    	is.putBack(nextTok);
    	
    	if( nextTok == token::BEGIN_BLOCK )
    	{
            
            uniquePtr<iEntry> ptr = makeUnique<dictionary>(key, parDict, is);
            
    		if( !parDict.addPtr(key, ptr ))
    		{
    			fatalErrorInFunction <<
    			"unable to add dicrionary " << key << 
    			" to dictionary " << parDict.globalName() <<endl;
    			fatalExit;
    		}
    	}
    	else
    	{
            uniquePtr<iEntry> ptr = makeUnique<dataEntry>(key, parDict, is);
            
    		if( !parDict.addPtr(key, ptr ) )
    		{
    			fatalErrorInFunction << 
    			"unable to add dataEntry " << key << 
    			" to dictionary " << parDict.globalName() <<endl;
    			fatalExit;
    		}
    	}
    }
    else
    {
        
    	if( nextTok.good() )
    	{
    		if(hasBlockToken)
            { 
                if(nextTok != token::END_BLOCK )
        		{
        			ioErrorInFile(is.name(), is.lineNumber())<<
        			"expecting a } but found "<< nextTok <<endl;
        			fatalExit;
        		}else
                {
                    return false;
                }	
        		
            }else
            {
                ioErrorInFile(is.name(), is.lineNumber())<<
                "not expecting " << nextTok <<endl;
                return false;
            }
    		
    	}
    	else if( !is.eof())
    	{
    		ioErrorInFile(is.name(), is.lineNumber())<<
    		"error in reading next token. \n";
    		fatalExit;
    	}
    	
    }

    return true;
}


bool pFlow::iEntry::writeKeyword
(
    iOstream& os
)const
{
    os.writeWordKeyword(keyword_);
    os.fatalCheck("pFlow::iEntry::writeKeyword");
    return true;
}


pFlow::iOstream& pFlow::operator << (iOstream& os, const iEntry& e)
{
    if(!e.write(os))
    {
        ioErrorInFile(os.name(), os.lineNumber())<<
        "Error in wrting to file \n";
        fatalExit;
    }
	return os;
}

pFlow::iIstream& pFlow::operator >> (iIstream& is, iEntry& e)
{	
    if( !e.read(is))
    {
        ioErrorInFile(is.name(), is.lineNumber())<<
        "Error in reading from file \n";
        fatalExit;
    }

	return is; 
}
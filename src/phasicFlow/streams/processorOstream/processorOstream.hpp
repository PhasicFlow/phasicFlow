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
#ifndef __processorOstream_hpp__
#define __processorOstream_hpp__

#include "Ostream.hpp"

namespace pFlow
{

/**
 * Output stream for MPI parallel run, when we need to 
 * know which the processor number in the output line. 
 * The processor number is shown as a prefix for the output line. 
 * 
 * It is based on OpenFOAM stream, with some modifications/simplifications
 * to be tailored to our needs.
 */
class processorOstream
:
    public Ostream
{
protected:

    /// Print prefix?
    bool printPrefix_	= false;

    /// Prefix word  
    word prefix_ 		= "";

    /// Output the prefix if required. 
    inline
    void checkForPrefix()
    {
    	if(printPrefix_ && prefix_.size())
    	{
    		Ostream::write(prefix_.c_str());
    		printPrefix_ = false;
    	}
    }

public:

    //// - Constructors
        
        /// From components 
        processorOstream(std::ostream& os, const word& streamName);

        /// No copy construct 
        processorOstream(const processorOstream&) = delete;

        /// No copy assignment
        void operator=(const processorOstream&) = delete;


    //// - Methods

        /// Set processor number to be used in the prefix. 
        word setPrefixNum(int procNum)
        {
            prefix_ = word("[")+int322Word(procNum)+word("] ");
            return prefix_;
        }

        /// Activate prefix for this stream.
        void activatePrefix()
        {
            printPrefix_ = true;
        }
        
        /// Write token to stream or otherwise handle it.
        /// return false if the token type was not handled by this method
        bool write(const token& tok)
        // to prevent compiler warning, this method is overriden agian 
        {
            return Ostream::write(tok);
        }

        /// Write character
        iOstream& write(const char c)override;

        /// Write character string
        iOstream& write(const char* str)override;

        /// Write word
        iOstream& write(const word& str)override;
    
        /// Write std::string surrounded by quotes.
        //  Optional write without quotes.
        iOstream& writeQuoted ( const word& str, const bool quoted=true ) override;

        /// Write int64
        iOstream& write(const int64 val) override;

        /// Write int32
        iOstream& write(const int32 val) override;

        /// Write int32
        iOstream& write(const int8 val) override;

        /// Write uint64
        iOstream& write(const uint64 val) override;

        /// Write uint32
        iOstream& write(const uint32 val) override;

        /// Write uint8
        iOstream& write(const uint8 val) override;

        /// Write float
        iOstream& write(const float val) override;

        /// Write double
        iOstream& write(const double val) override;

        /// Write double
        iOstream& write(const size_t val) override;
        
        /// Write a block of binray data 
        iOstream& write(const char* binaryData, std::streamsize count) override;

        /// Add indentation characters
        void indent() override;

        processorOstream& setColor(const char* colorCode);


}; // processorOstream



} // pFlow


#endif // __processorOstream_hpp__

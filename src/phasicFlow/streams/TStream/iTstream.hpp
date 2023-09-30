
#ifndef __iTstream_hpp__
#define __iTstream_hpp__

#include "iIstream.hpp"
#include "tokenList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace pFlow
{

//- helper functions declearation
inline bool validTokenForStream(const token tok);

inline bool isBeginToken(const token& tok);

inline bool isEndToken(const token& tok);


/**
 * Input token stream. 
 * 
 * It is based on OpenFOAM stream, with some modifications/simplifications
 * to be tailored to our needs.
 */
class iTstream
:
    public iIstream
{
protected:
    
    /// name of the stream
    word    name_;

    /// list of tokens in this stream
    tokenList  tokenList_;

    /// current token
    tokenList::iterator currentToken_;

    /// check if end of list is reached 
    bool isLastToken();

    /// rewind the stream 
    void setFirstToken();

    /// check for valid tokens in the tokenList 
    void validate();

public:

    //// - Constructors 

        /// construct with a name
        iTstream(const word& streamName);

        /// construct with name and copy
        iTstream(const word& streamName, const tokenList& tList);

        /// construct with name and move 
        iTstream(const word& streamName, tokenList && tList);

        /// copy construct
        iTstream(const iTstream&) = default;
        
        /// move construct 
        iTstream(iTstream&&) = default;

        /// copy assignment
        iTstream& operator=(const iTstream&) = default;

        /// move assignment 
        iTstream& operator=(iTstream&&) = default;

        /// copy assignment from tokenList
        void operator=(const tokenList& tList);
        
        /// move assignment from tokenList
        void operator=(tokenList&& tList);

        /// Destructor
        virtual ~iTstream() = default;


    //// - Member Functions   

        /// Return the name of the stream
        virtual const word& name() const;

        /// Return non-const access to the name of the stream
        virtual word& name();

        /// Return next token from stream
        virtual iIstream& read(token& t)override;

        /// Read a character
        virtual iIstream& read(char& c)override;

        /// Read a word
        virtual iIstream& read(word& str)override;

        /// Read a string
        virtual iIstream& readString(word& str)override;

        /// Read a int64
        virtual iIstream& read(int64&) override;

        /// Read a int32
        virtual iIstream& read(int32&) override;
        
        /// Read a int8
        virtual iIstream& read(int8&) override;

        /// Read a uint64
        virtual iIstream& read(uint64&) override;

        /// Read a uint32
        virtual iIstream& read(uint32&) override;

        /// Read a uint8
        virtual iIstream& read(uint8&) override;

        /// Read a floatScalar
        virtual iIstream& read(float&) override;

        /// Read a doubleScalar
        virtual iIstream& read(double&) override;

        iIstream& read(char* buffer, std::streamsize count) override;

        /// Rewind the stream so that it may be read again
        virtual void rewind();

        /// reset the iTstream and make the stream empty
        virtual void reset();

        /// const access to token list
        const tokenList& tokens()const;

        /// size
        size_t size()const; 

        /// size
        size_t numTokens()const;
        
        /// append a list of tokens to the end of tokens
        //   and rewind the stream
        void appendTokens(const tokenList & tList);

        /// append token to the end of token and rewind the stream 
        void appendToken(const token& t);
   
        /// Return flags of output stream
        ios_base::fmtflags flags() const
        {
            return ios_base::fmtflags(0);
        }

        /// Set flags of stream
        ios_base::fmtflags flags(const ios_base::fmtflags)
        {
            return ios_base::fmtflags(0);        
        }

};

#include "helperTstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

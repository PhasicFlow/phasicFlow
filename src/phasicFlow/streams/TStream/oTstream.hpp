
#ifndef __oTstream_hpp__
#define __oTstream_hpp__

#include "tokenList.hpp"
#include "iOstream.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace pFlow
{


// helper functions declearation
inline bool validTokenForStream(const token tok);

inline bool isBeginToken(const token& tok);

inline bool isEndToken(const token& tok);


class oTstream
:
    public iOstream
{

protected:

    // - name of stream
    word    name_;

    // - tokenList
    tokenList  tokenList_;

public:
    
    //// - Constructors

        // - emtpy stream with a name
        oTstream(const word& nm);
        
        // - copy construcotr 
        oTstream(const oTstream& src);

        // - move construct 
        oTstream(oTstream&&) = default;

        // - destructor 
        virtual ~oTstream() = default;

    
    //// -  Methods
    
        // give const access 
        const tokenList& tokens()const;

        // give access 
        tokenList& tokens();
    

    //// - Write

        //- Write token to stream or otherwise handle it.
        //  return false if the token type was not handled by this method
        virtual bool write(const token& tok);

        //- Write single character. Whitespace is suppressed.
        virtual iOstream& write(const char c);

        //- Write the word-characters of a character string.
        //  Sends as a single char, or as word.
        virtual iOstream& write(const char* str);

        //- Write word
        virtual iOstream& write(const word& str);

        
        //- Write std::string surrounded by quotes.
        //  Optional write without quotes.
        virtual iOstream& writeQuoted(const std::string& str, const bool quoted=true );

        //- Write int64
        virtual iOstream& write(const int64 val) override;

        //- Write int32
        virtual iOstream& write(const int32 val) override;

        
        //- Write label
        virtual iOstream& write(const label val) override;

        //- Write uint32
        virtual iOstream& write(const uint32 val) override;

        //- Write uint16
        virtual iOstream& write(const uint16 val) override;

        //- Write float
        virtual iOstream& write(const float val) override;

        //- Write double
        virtual iOstream& write(const double val) override;

        /// Write a block of binray data
        iOstream& write(
            const char* binaryData, 
            std::streamsize count) override;

        // - append token to the stream
        virtual void append(const token& tok);
        
        // - append a list of tokens to the stream
        virtual void append(const tokenList& tLisk);
        
    

    //// - Stream state functions

        //- Reset the output buffer and rewind the stream
        void reset();

        //- Rewind the output stream
        virtual void rewind();

        //- Add indentation characters
        virtual void indent()
        {}

        //- Flush stream
        virtual void flush()
        {}

        //- Add newline and flush stream
        virtual void endl()
        {}

        //- Get the current padding character
        //  \return previous padding character
        virtual char fill() const
        {
            return 0;
        }

        //- Set padding character for formatted field up to field width
        virtual char fill(const char)
        {
            return 0;
        }

        //- Get width of output field
        virtual int width() const
        {
            return 0;
        }

        //- Set width of output field
        //  \return previous width
        virtual int width(const int)
        {
             return 0;
        }

        //- Get precision of output field
        virtual int precision() const
        {
             return 0;
        }

        //- Set precision of output field
        //  \return old precision
        virtual int precision(const int)
        {
             return 0;
        }

        //- Return flags of output stream
        virtual ios_base::fmtflags flags() const
        {
            return ios_base::fmtflags(0);
        }

        //- Set flags of stream
        ios_base::fmtflags flags(const ios_base::fmtflags)
        {
            return ios_base::fmtflags(0);
        }

};

#include "helperTstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace pFlow

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

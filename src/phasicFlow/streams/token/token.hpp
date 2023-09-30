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
#ifndef __token_hpp__
#define __token_hpp__


#include "bTypes.hpp"



namespace pFlow
{

/// Forward Declarations
class token;
class iIstream;
class iOstream;

iOstream& operator<<(iOstream& os, const token& tok);


/**
 *  Token class 
 *  based on OpenFOAM stream, with some modifications/simplifications
 *  to be tailored to our needs 
 */
class token
{
public:

    /// Enumeration defining the types of token.
    /// Since these values are also used to tag content in Pstream,
    /// the maximum number of types is limited to 30.
    enum tokenType
    {
        UNDEFINED = 0,    //!< An undefined token-type

        // Fundamental types
        FLAG,             /// stream flag (1-byte bitmask)
        PUNCTUATION,      /// single character punctuation
        BOOL,             /// boolean type
        INT64,            /// int64 (integer) type
        FLOAT,            /// float (single-precision) type
        DOUBLE,           /// double (double-precision) type

        // Pointer types
        WORD,             /// A pFlow::word
        STRING, 		  /// A string whth double quuote
        DIRECTIVE,        /// A dictionary \c \#directive (word variant)
        VARIABLE,         /// A dictionary \c \$variable (string variant)

        ERROR,            /// A token error encountered
    };


    /// Stream or output control flags (1-byte width)
    enum flagType
    {
        NO_FLAG = 0,   /// No flags
        ASCII   = 1,   /// ASCII-mode stream
        BINARY  = 2    /// BINARY-mode stream
    };


    /// Standard punctuation tokens (a character)
    enum punctuationToken : char
    {
        NULL_TOKEN     = '\0',  /// Nul character
        SPACE          = ' ',   /// Space [isspace]
        TAB            = '\t',  /// Tab [isspace]
        NL             = '\n',  /// Newline [isspace]

        END_STATEMENT  = ';',   /// End entry [#isseparator]
        BEGIN_LIST     = '(',   /// Begin list [#isseparator]
        END_LIST       = ')',   /// End list [#isseparator]
        BEGIN_SQR      = '[',   /// Begin dimensions [#isseparator]
        END_SQR        = ']',   /// End dimensions [#isseparator]
        BEGIN_BLOCK    = '{',   /// Begin block [#isseparator]
        END_BLOCK      = '}',   /// End block [#isseparator]
        COLON          = ':',   /// Colon [#isseparator]
        COMMA          = ',',   /// Comma [#isseparator]
        DOLLAR         = '$',   /// Dollar - start variable
        SQUOTE         = '\'',  /// Single quote
        DQUOTE         = '"',   /// Double quote

        SUBTRACT       = '-',   /// Subtract or start of negative number
        DIVIDE         = '/',   /// Divide [#isseparator]

        BEGIN_STRING   = DQUOTE, /// Begin string with double quote
        END_STRING     = DQUOTE  /// End string with double quote
    };

    /// An undefined token
    static const inline token undefinedToken();

    static token endList()
    {
        return token(punctuationToken::END_LIST);
    }

    static token beginList()
    {
        return token(punctuationToken::BEGIN_LIST);
    }

    static token endStatement()
    {
        return token(punctuationToken::END_STATEMENT);
    }

    static token beginBlock()
    {
        return token(punctuationToken::BEGIN_BLOCK);
    }

    static token endBlocK()
    {
        return token(punctuationToken::END_BLOCK);
    }

    static token beginSquare()
    {
        return token(punctuationToken::BEGIN_SQR);
    }

    static token endSquare()
    {
        return token(punctuationToken::END_SQR);
    }

    static token space()
    {
        return token(punctuationToken::SPACE);
    }

    static token newLine()
    {
        return token(punctuationToken::NL);
    }

private:

    /// A %union of token types
    union content
    {
        // Fundamental values. Largest first for any {} initialization.
        int64_t int64Val;

        int flagVal;   // bitmask - stored as int, not enum
        punctuationToken punctuationVal;
        float floatVal;
        double doubleVal;

        // Pointers
        word* wordPtr;
        word* stringPtr;
    };


    //// -  Private Data

    /// The data content (as a union).
    //  For memory alignment this should appear as the first member.
    content data_;

    /// The token type
    tokenType type_;

    /// Line number in the file the token was read from
    int32 lineNumber_;


    // Private Member Functions

    /// Set as UNDEFINED and zero the union content without any checking
    inline void setUndefined();

    /// Parse error, expected 'expected', found ...
    void parseError(const char* expected) const;


public:


    //// - Constructors

        /// Default construct, initialized to an UNDEFINED token.
        inline constexpr token() noexcept;

        /// Copy construct
        inline token(const token& t);

        /// Move construct. The original token is left as UNDEFINED.
        inline token(token&& t);

        /// Construct punctuation character token
        inline explicit token(punctuationToken p, int32 lineNumber=0);

        /// Construct int64 token
        inline explicit token(const uint64 val, int32 lineNumber=0);

        /// Construct int64 token
        inline explicit token(const uint32 val, int32 lineNumber=0);

        /// Construct int8 token
        inline explicit token(const uint8 val, int32 lineNumber=0);

        /// Construct int64 token
        inline explicit token(const int64 val, int32 lineNumber=0);

        /// Construct int64 token
        inline explicit token(const int32 val, int32 lineNumber=0);

        /// Construct int64 token
        inline explicit token(const int8 val, int32 lineNumber=0);

        /// Construct float token
        inline explicit token(const float val, int32 lineNumber=0);

        /// Construct double token
        inline explicit token(const double val, int32 lineNumber=0);

        /// Copy construct word & string token
        inline explicit token(const word& w, int32 lineNumber=0, bool isString = false);


        /// Move construct word & string token
        inline explicit token(word&& w, int32 lineNumber=0, bool isString = false);

        /// Construct from iIstream
        explicit token(iIstream& is);


        /// Destructor
        inline ~token();


    //// - Static Functions

        /// Create a bool token.
        inline static token boolean(bool on);

        /// Create a token with stream flags, no sanity check
        ///
        ///  \param bitmask the flags to set
        inline static token flag(int bitmask);

        /// True if the character is a punctuation separator (eg, in ISstream).
        ///  Since it could also start a number, SUBTRACT is not included as
        ///  a separator.
        ///
        ///  \param c the character to test, passed as int for consistency with
        ///      isdigit, isspace etc.
        inline static bool isseparator(int c);


    //// - Member Functions

    // - Status

        /// Return the name of the token type
        word name() const;

        /// Return the token type
        inline tokenType type() const;

        /// Change the token type, for similar types.
        ///  This can be used to change between string-like variants
        ///  (eg, STRING, VARIABLE, etc)
        ///  To change types entirely (eg, STRING to DOUBLE),
        ///  use the corresponding assignment operator.
        ///
        ///  \return true if the change was successful or no change was required
        inline bool setType(const tokenType tokType);

        /// The line number for the token
        inline int32 lineNumber() const;

        /// The line number for the token
        inline int32& lineNumber();

        /// True if token is not UNDEFINED or ERROR
        inline bool good() const;

        /// Token is UNDEFINED
        inline bool undefined() const;

        /// Token is ERROR
        inline bool error() const;

        /// Token is BOOL
        inline bool isBool() const;

        /// Token is FLAG
        inline bool isFlag() const;

        /// Token is PUNCTUATION
        inline bool isPunctuation() const;

        /// Token is PUNCTUATION and isseparator
        inline bool isSeparator() const;

        /// Token is end statement 
        inline bool isEndStatement() const;

        /// Token is end endBlock 
        inline bool isEndBlock()const;

        /// Token is int64
        inline bool isInt64() const;

        /// Token is int32
        inline bool isInt32() const;

        /// Token is float
        inline bool isFloat() const;

        /// Token is double
        inline bool isDouble() const;

        /// Token is float or double
        inline bool isReal() const;

        /// Token is int, float or duble
        inline bool isNumber() const;

        /// Token is word or DIRECTIVE word
        inline bool isWord() const;

        /// Token is DIRECTIVE (word variant)
        inline bool isDirective() const;

        /// Token is STRING, VARIABLE or VERBATIM string
        inline bool isString() const;

        /// Token is VARIABLE (string variant)
        inline bool isVariable() const;

        /// Token is WORD, DIRECTIVE, STRING, VARIABLE or VERBATIM
        inline bool isStringType() const;


    //- Access

        /// Return boolean token value.
        ///  Report FatalIOError and return false if token is not BOOL or INT64
        inline bool boolToken() const;

        /// Return flag bitmask value.
        ///  Report FatalIOError and return NO_FLAG if token is not FLAG
        inline int flagToken() const;

        /// Return punctuation character.
        ///  Report FatalIOError and return \b \\0 if token is not PUNCTUATION
        inline punctuationToken pToken() const;

        /// Return int64 value.
        //  Report FatalIOError and return \b 0 if token is not INT64
        inline int64 int64Token() const;

        /// Return int32 value.
        ///  Report FatalIOError and return \b 0 if token is not INT64
        inline int32 int32Token() const;

        /// Return float value.
        ///  Report FatalIOError and return \b 0 if token is not FLOAT
        inline float floatToken() const;

        /// Return double value.
        ///  Report FatalIOError and return \b 0 if token is not DOUBLE
        inline double doubleToken() const;

        /// Return float or double value.
        ///  Report FatalIOError and return \b 0 if token is not a
        ///  FLOAT or DOUBLE
        inline real realToken() const;

        /// Return int64, float or double value.
        ///  Report FatalIOError and return \b 0 if token is not a
        ///  INT64, FLOAT or DOUBLE
        inline real number() const;

        /// Return const reference to the word contents.
        ///  Report FatalIOError and return \b "" if token is not a
        ///  WORD or DIRECTIVE
        inline const word& wordToken() const;

        /// Return const reference to the string contents.
        ///  Report FatalIOError and return \b "" if token is not a
        ///  STRING, VARIABLE, VERBATIM or an upcast WORD or DIRECTIVE
        inline const word& stringToken() const;


    //- Edit

        /// Reset token to UNDEFINED and clear any allocated storage
        inline void reset();

        /// Clear token and set to be ERROR.
        inline void setBad();

        /// Swap token contents: type, data, line-number
        inline void swap(token& tok);


    //- Assignment

        /// Copy assign
        inline void operator=(const token& tok);

        /// Move assign
        inline void operator=(token&& tok);

        /// Copy assign from punctuation
        inline void operator=(const punctuationToken p);

        /// Copy assign from int64
        inline void operator=(const int64 val);

         /// Copy assign from int32
        inline void operator=(const int32 val);

        /// Copy assign from float
        inline void operator=(const float val);

        /// Copy assign from double
        inline void operator=(const double val);

        /// Copy assign from word
        inline void operator=(const word& w);

                

        /// Move assign from word
        inline void operator=(word&& w);

        
    //- Equality

        inline bool operator==(const token& tok) const;
        inline bool operator==(const punctuationToken p) const;
        inline bool operator==(const int64 val) const;
        inline bool operator==(const int32 val) const;
        inline bool operator==(const float val) const;
        inline bool operator==(const double val) const;
        inline bool operator==(const word& w) const;


    //- Inequality

        inline bool operator!=(const token& tok) const;
        inline bool operator!=(const punctuationToken p) const;
        inline bool operator!=(const int64 val) const;
        inline bool operator!=(const int32 val) const;
        inline bool operator!=(const float val) const;
        inline bool operator!=(const double val) const;
        inline bool operator!=(const word& w) const;

        iOstream& printInfo(iOstream& os)const;

        std::ostream& printInfo(std::ostream& os)const;
    
    //- IOstream Operators

        friend iOstream& operator<<(iOstream& os, const token& tok);
        friend iOstream& operator<<(iOstream& os, const punctuationToken& pt);

        // mostly used for debuging and developement 
        friend std::ostream& operator<<(std::ostream& os, const token& tok);
        friend std::ostream& operator<<(std::ostream& os, const punctuationToken& pt); 
        
        void operator=(word*) = delete;

};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// IOstream Operators

iIstream& operator>>(iIstream& is, token& tok);
iOstream& operator<<(iOstream& os, const token::punctuationToken& pt);
std::ostream&  operator<<(std::ostream& os, const token::punctuationToken& pt);
std::ostream& operator<<(std::ostream& os, const token& tok);


inline token endListToken()
{
    return token::endList();
}

inline token beginListToken()
{
    return token::beginList();
}

inline token endStatementToken()
{
    return token::endStatement();
}

inline token beginBlockToken()
{
    return token::beginBlock();
}

inline token endBlocKToken()
{
    return token::endBlocK();
}

inline token spaceToken()
{
    return token::space();
}

inline token newLineToken()
{
    return token::newLine();
}


} // End namespace pFlow


#include "tokenI.hpp"


#endif

#ifndef __helperTstream_hpp__
#define __helperTstream_hpp__



inline bool validTokenForStream(const token tok)
{
	if( tok.good() && !tok.isPunctuation() )return true;
	if( tok == token::SPACE) return false;
	if( tok == token::TAB) return false;
	if( tok == token::NL) return false;
	if( tok == token::NULL_TOKEN )return false;

	return true;
}

inline bool isBeginToken(const token& tok)
{
    if( tok.good() && !tok.isPunctuation() )return false;
    if( tok == token::BEGIN_LIST) 	return true;
    if( tok == token::BEGIN_BLOCK)	return true;
    if( tok == token::BEGIN_SQR)	return true;
    return false;
}

inline bool isEndToken(const token& tok)
{
    if( tok.good() && !tok.isPunctuation() )return false;
    if( tok == token::END_LIST) return true;
    if( tok == token::END_BLOCK)return true;
    if( tok == token::END_SQR)	return true;
    return false;
}

#endif // __helperTstream__

#include <string>
#include "token.h"



Token::Token( string buffer, string delimiter )
{
	Tokenize( buffer, delimiter );
}


Token::Token( string buffer )
{
	Tokenize( buffer, " " );
}


int Token::Tokenize( string buffer, string delimiter )
{
	int dpos = -1, ddpos = -1, t = 0;
	string thisToken, doubleDelim, lhs, rhs;

	doubleDelim = delimiter + delimiter;
	
	ddpos = buffer.find( "  " );
	while( ddpos > -1 )
	{
		lhs = buffer.substr( 0, ddpos );
		rhs = buffer.substr( ++ddpos, buffer.length() );
		buffer = lhs + rhs;
		ddpos = buffer.find( doubleDelim );
	}
	
	if( ( dpos = buffer.find( "\r" ) ) && dpos > -1 )
		buffer[dpos] = '\0';

	if( ( dpos = buffer.find( "\n" ) ) && dpos > -1 )
		buffer[dpos] = '\0';

	buffer.append( delimiter );

	for( t = 0; ( dpos = buffer.find( delimiter ) ) && dpos > -1; t++ ) 
	{
		tokenArray.resize( t+1 );
		thisToken = buffer.substr( 0, dpos );
		tokenArray[t] = thisToken;
		buffer = buffer.substr( dpos+1, buffer.length() );
	}

	return 0;
}


int Token::Tokenize( string buffer )
{
	return Tokenize( buffer, " " );
}


string Token::Assemble()
{
	string fullBuffer;
	int i;

	for( i = 0; i < this->numTokens(); i++ )
	{
		if( i > 0 ) 
			fullBuffer.append( " " );
		fullBuffer.append( tokenArray[i] );
	}
	return fullBuffer;
}


string Token::Assemble( int sPos )
{
	string fullBuffer;
	int i;

	sPos--;

	for( i = sPos; i < this->numTokens(); i++ )
	{
		if( i > sPos )
			fullBuffer.append( " " );
		fullBuffer.append( tokenArray[i] );
	}
	return fullBuffer;
}


string Token::Assemble( int sPos, int ePos )
{
	string fullBuffer;
	int i;

	sPos--;
	ePos--;

	for( i = sPos; i < ePos && ( i < this->numTokens() ); i++ )
	{
		if( i > sPos )
			fullBuffer.append( " " );
		fullBuffer.append( tokenArray[i] );
	}
	return fullBuffer;
}


void Token::operator= ( string &buffer )
{
	Tokenize( buffer, " " );
}

#ifndef __TOKEN_H__
#define __TOKEN_H__ "$Id: token.h,v 1.7 2002/01/25 20:56:59 bcline Exp $"

#include <string>
#include <vector>

using namespace std;


class Token
{
	public:

		Token() {};
		Token( string buffer, string delimiter );
		Token( string buffer );
		~Token() {}

		int Tokenize( string buffer, string delimiter );
		int Tokenize( string buffer );

		int numTokens()
			{ return tokenArray.size(); }

		string Assemble();
		string Assemble( int sPos );
		string Assemble( int sPos, int ePos );

		string &operator [] ( int index )
			{ return tokenArray[index]; }

		void operator= ( string &right );


	private:

		vector<string> tokenArray;

};


#endif // __TOKEN_H__

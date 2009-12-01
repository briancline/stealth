#include "stealth.h"
#include "cmdmap.h"
#include "network.h"

using namespace std;

cmdStatusType cmd_TRANSLATE ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Client *user;
	Server *serv;
	int numUsers = 0, i = 0;
	string servNum, userNum;

	if( tokens.numTokens() < 5 )
		return CMD_SYNTAX;
	
	numUsers = tokens.numTokens() - 4;

	if( numUsers == 0 )
		numUsers = 1;

	for( i = 0; i < numUsers; i++ )
	{
		userNum = tokens[i + 4];
		user = Net->FindClientByNum( userNum );

		if( user == NULL )
		{
			Net->Send( "%s O %s :%s was not found.\n", nDst.c_str(), nSrc.c_str(), userNum.c_str() );
			continue;
		}
		else
		{
			servNum = getServNum( userNum );
			serv = Net->FindServerByNum( servNum );
			Net->Send( "%s O %s :%s is %s!%s@%s on %s\n", nDst.c_str(), nSrc.c_str(), userNum.c_str(),
				user->GetNick().c_str(), user->GetUserName().c_str(),
				user->GetHostName().c_str(), serv->GetName().c_str() );
		}
	}

	Report( CMD_TRANSLATE, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


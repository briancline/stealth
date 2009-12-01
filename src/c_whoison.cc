#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_WHOISON ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Channel *chan = Net->FindChannelByName( tokens[4] );
	int i = 0, count = 0;
	string userList;
	Token users;

	if( chan == NULL )
	{
		Net->Send( "%s O %s :Channel %s does not exist.\n", nDst.c_str(), nSrc.c_str(), tokens[4].c_str() );
		return CMD_ERROR;
	}

	Net->Send( "%s O %s :Users on \002%s\002:\n", nDst.c_str(), nSrc.c_str(), chan->GetName().c_str() );

	userList = chan->GetUsersString();
	users.Tokenize( userList );
	userList.erase();
	
	for( i = 0; i < users.numTokens(); i++ )
	{
		userList.append( " " );
		userList.append( users[i] );
		count++;

		if( count == 20 )
		{
			Net->Send( "%s O %s :   %s\n", nDst.c_str(), nSrc.c_str(), userList.c_str() );
			userList.erase();
			count = 0;
		}
	}

	if( count > 0 )
		Net->Send( "%s O %s :   %s\n", nDst.c_str(), nSrc.c_str(), userList.c_str() );

	Report( CMD_WHOISON, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


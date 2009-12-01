#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_DEOP ( Numeric nSrc, Numeric nDst, Token tokens )
{
	string users, deopString;
	int numUsers = tokens.numTokens()-5, i = 0, userCount = 0;
	Channel *chan = Net->FindChannelByName( tokens[4] );
	ChannelClient *chanUser;
	Client *user;
	Token nickList;

	if( chan == NULL )
	{
		Net->Send( "%s O %s :Channel %s does not exist.\n", nDst.c_str(), nSrc.c_str(), 
			tokens[4].c_str() );
		return CMD_ERROR;
	}

	if( numUsers == 0 )
	{
		user = Net->FindClientByNum( nSrc );
		nickList.Tokenize( user->GetNick() );
	}
	else
	{
		nickList.Tokenize( tokens.Assemble( 6 ) );
	}
	numUsers = nickList.numTokens();

	for( i = 0; i < numUsers; i++ )
	{
		user = Net->FindClientByNick( nickList[i] );

		if( user == NULL )
		{
			Net->Send( "%s O %s :Client %s does not exist.\n", nDst.c_str(), nSrc.c_str(),
				user->GetNick().c_str() );
			continue;
		}

		if( userCount > 0 )
			users.append( " " );

		users.append( user->GetNumeric() );
		deopString.append( "o" );
		userCount++;

		chanUser = chan->FindUser( user->GetNumeric() );
		chanUser->RemOp();

		if( userCount == 6 )
		{
			Net->Send( "%s M %s -%s %s\n", Net->GetConfig("SERVNUM"), chan->GetName().c_str(),
				deopString.c_str(), users.c_str() );
			deopString.erase();
			users.erase();
			userCount = 0;
		}
	}

	if( userCount > 0 )
	{
		Net->Send( "%s M %s -%s %s\n", Net->GetConfig("SERVNUM"), chan->GetName().c_str(),
			deopString.c_str(), users.c_str() );
		deopString.erase();
		users.erase();
		userCount = 0;
	}
	
	Report( CMD_DEOP, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


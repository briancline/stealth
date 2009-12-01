#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_VOICE ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Channel *chan = Net->FindChannelByName( tokens[4] );
	Client *user;
	ChannelClient *chanUser;
	Token nickList( tokens.Assemble( 6 ) );
	int numUsers = nickList.numTokens(), i = 0, userCount = 0;
	string users, voiceString;

	if( chan == NULL )
	{
		Net->Send( "%s O %s :Channel %s does not exist.\n", nDst.c_str(), nSrc.c_str(), 
			tokens[4].c_str() );
		return CMD_ERROR;
	}

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
		voiceString.append( "v" );
		userCount++;

		chanUser = chan->FindUser( user->GetNumeric() );
		chanUser->SetVoice();

		if( userCount == 6 )
		{
			Net->Send( "%s M %s +%s %s\n", Net->GetConfig("SERVNUM"), chan->GetName().c_str(),
				voiceString.c_str(), users.c_str() );
			voiceString.erase();
			users.erase();
			userCount = 0;
		}
	}

	if( userCount > 0 )
	{
		Net->Send( "%s M %s +%s %s\n", Net->GetConfig("SERVNUM"), chan->GetName().c_str(),
			voiceString.c_str(), users.c_str() );
		voiceString.erase();
		users.erase();
		userCount = 0;
	}
	
	Report( CMD_VOICE, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_I ( Numeric nSrc, Numeric nDst, Token params )
{
	Client *user = Net->FindClientByNum( nSrc ), 
	       *bot = Net->FindClientByNick( params[2] );
	string channel = params[3].substr( 1, params[3].length() );
	Channel *chan = Net->FindChannelByName( channel );
	ChannelClient *chanClient;

	assert( user != NULL );

	Net->Send( "%s WA :%s!%s@%s is inviting me to %s\n", bot->GetNumeric().c_str(),
		user->GetNick().c_str(), user->GetUserName().c_str(), user->GetHostName().c_str(),
		channel.c_str() );

	// only join if an oper invites the bot.
	if( user->IsOper() )
	{
		if( chan == NULL )
		{
			Net->AddChannel( channel, time( NULL ) );
			chan = Net->FindChannelByName( channel );

			Net->Send( "%s C %s %ld\n", bot->GetNumeric().c_str(), channel.c_str(), chan->GetTS() );
		}
		else
		{
			Net->Send( "%s J %s\n", bot->GetNumeric().c_str(), channel.c_str() );
			Net->Send( "%s M %s +o %s\n", Net->GetConfig( "SERVNUM" ), channel.c_str(), 
				bot->GetNumeric().c_str() );
		}
		
		chan->AddUser( bot->GetNumeric() );

		chanClient = chan->FindUser( bot->GetNumeric() );
		chanClient->SetOp();

	}
	
	Report( SRV_INVITE, nSrc, 0, bot->GetNick().c_str(), channel.c_str() );
	
	return MSG_SUCCESS;
}


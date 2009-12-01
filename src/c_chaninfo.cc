#include "stealth.h"
#include "network.h"
#include "cmdmap.h"
#include "channel.h"
#include "client.h"

using namespace std;

cmdStatusType cmd_CHANINFO ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Channel *chan;
	string chanName = tokens[4];

	chan = Net->FindChannelByName( chanName );

	if( chan == NULL )
	{
		Net->Send( "%s O %s :Channel %s does not exist.\n", nDst.c_str(), nSrc.c_str(), 
			chanName.c_str() );
		return CMD_ERROR;
	}

	Net->Send( "%s O %s :Information for %s:\n", nDst.c_str(), nSrc.c_str(), 
		chan->GetName().c_str() );
	Net->Send( "%s O %s :  Create time: %s\n", nDst.c_str(), nSrc.c_str(), 
		chan->GetCreateTime().c_str() );
	Net->Send( "%s O %s :  Topic:       %s\n", nDst.c_str(), nSrc.c_str(), 
		chan->GetTopic().c_str() );
	Net->Send( "%s O %s :  Modes:       %s\n", nDst.c_str(), nSrc.c_str(), 
		chan->GetModeString().c_str() );
	Net->Send( "%s O %s :  Users:       %s\n", nDst.c_str(), nSrc.c_str(), 
		chan->GetUsersString().c_str() );
	Net->Send( "%s O %s :  Bans:        %s\n", nDst.c_str(), nSrc.c_str(), 
		chan->GetBansString().c_str() );

	Report( CMD_CHANINFO, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_INVITE ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Client *user = Net->FindClientByNum( nSrc );
	Channel *chan = Net->FindChannelByName( tokens[4] );
	ChannelClient *chanUsr;

	if( chan == NULL )
	{
		Net->Send( "%s O %s :Channel %s does not exist.\n", nDst.c_str(), nSrc.c_str(), tokens[4].c_str() );
		return CMD_ERROR;
	}

	if( strLower(tokens[4]) == strLower(Net->GetConfig("INFOCHAN")) )
	{
		Net->Send( "%s O %s :You must use the INVITEME command for that channel.\n", nDst.c_str(),
				nSrc.c_str() );
		return CMD_ERROR;
	}

	if( chan->FindUser( nSrc ) != NULL )
	{
		Net->Send( "%s O %s :You are already on %s.\n", nDst.c_str(), nSrc.c_str(),
			chan->GetName().c_str() );
		return CMD_ERROR;
	}
	
	chanUsr = chan->FindUser( nDst );
	if( chanUsr == NULL )
	{
		Net->Send( "%s J %s\n", nDst.c_str(), chan->GetName().c_str() );
//		Net->Send( "%s M %s +o %s\n", Net->GetConfig( "SERVNUM" ), chan->GetName().c_str(), nDst.c_str() );
		Net->Send( "%s I %s :%s\n", nDst.c_str(), user->GetNick().c_str(), tokens[4].c_str() );
		Net->Send( "%s L %s\n", nDst.c_str(), chan->GetName().c_str() );
	}
	else
	{
		if( !chanUsr->IsOp() )
			Net->Send( "%s M %s +o %s\n", Net->GetConfig( "SERVNUM" ), chan->GetName().c_str(), nDst.c_str() );

		Net->Send( "%s I %s :%s\n", nDst.c_str(), user->GetNick().c_str(), tokens[4].c_str() );
	}

	Report( CMD_INVITE, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}

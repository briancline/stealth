#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_KICK ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Channel *chan = Net->FindChannelByName( tokens[4] );
	Client *user = Net->FindClientByNick( tokens[5] );
	string reason = tokens.Assemble( 7 );

	if( chan == NULL )
	{
		Net->Send( "%s O %s :Channel %s does not exist.\n", nDst.c_str(), nSrc.c_str(), tokens[4].c_str() );
		return CMD_ERROR;
	}

	if( user == NULL )
	{
		Net->Send( "%s O %s :Client %s does not exist.\n", nDst.c_str(), nSrc.c_str(), tokens[5].c_str() );
		return CMD_ERROR;
	}

	Net->Send( "%s K %s %s :%s\n", Net->GetConfig("SERVNUM"), chan->GetName().c_str(), user->GetNumeric().c_str(), reason.c_str() );

	Report( CMD_KICK, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


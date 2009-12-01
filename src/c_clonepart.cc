#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_CLONEPART ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Client *user = Net->FindClientByNick( tokens[4] );
	Channel *chan = Net->FindChannelByName( tokens[5] );

	if( user == NULL )
	{
		Net->Send( "%s O %s :Clone %s does not exist.\n", nDst.c_str(), nSrc.c_str(),
			tokens[4].c_str() );
		return CMD_ERROR;
	}
	if( chan == NULL )
	{
		Net->Send( "%s O %s :Channel %s does not exist.\n", nDst.c_str(), nSrc.c_str(),
			tokens[5].c_str() );
		return CMD_ERROR;
	}

	Net->Send( "%s L %s\n", user->GetNumeric().c_str(), chan->GetName().c_str() );
	chan->RemUser( user->GetNumeric() );
	
	Report( CMD_CLONEPART, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


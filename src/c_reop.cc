#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_REOP ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Client *user = Net->FindClientByNick( tokens[4] );

	if( user == NULL )
	{
		Net->Send( "%s O %s :Client %s does not exist.\n", nDst.c_str(), nSrc.c_str(), 
			tokens[4].c_str() );
		return CMD_ERROR;
	}
	else if( !user->IsService() )
	{
		Net->Send( "%s O %s :Client %s is not a service bot (+k).\n", nDst.c_str(),
			nSrc.c_str(), user->GetNick().c_str() );
		return CMD_ERROR;
	}

	user->OpOnAllChans();
	
	Report( CMD_REOP, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


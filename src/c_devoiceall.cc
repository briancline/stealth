#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_DEVOICEALL ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Channel *chan = Net->FindChannelByName( tokens[4] );

	if( chan == NULL )
	{
		Net->Send( "%s O %s :Channel %s does not exist.\n", nDst.c_str(), nSrc.c_str(), 
			tokens[4].c_str() );
		return CMD_ERROR;
	}

	chan->DevoiceAll();
	
	Report( CMD_DEVOICEALL, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


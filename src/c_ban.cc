#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_BAN ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Channel *chan = Net->FindChannelByName( tokens[4] );
	string modeBuf, paramBuf;
	int i = 0, banCount = 0;
	
	if( chan == NULL )
	{
		Net->Send( "%s O %s :Channel %s does not exist.\n", nDst.c_str(), nSrc.c_str(), 
			tokens[5].c_str() );
		return CMD_ERROR;
	}

	for( i = 5; i < tokens.numTokens(); i++ )
	{
		chan->AddBan( tokens[i] );
		modeBuf += "b";
		paramBuf += " " + tokens[i];
		banCount++;

		if( banCount == 6 )
		{
			Net->Send( "%s M %s +%s%s\n", Net->GetConfig( "SERVNUM" ), chan->GetName().c_str(),
				modeBuf.c_str(), paramBuf.c_str() );
			modeBuf.erase();
			paramBuf.erase();
			banCount = 0;
		}
	}

	if( banCount > 0 )
	{
		Net->Send( "%s M %s +%s%s\n", Net->GetConfig( "SERVNUM" ), chan->GetName().c_str(),
			modeBuf.c_str(), paramBuf.c_str() );
	}
	
	Report( CMD_BAN, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}

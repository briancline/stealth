#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_CHLEVEL ( Numeric nSrc, Numeric nDst, Token tokens )
{
	int cmd = 0, newLevel, oldLevel;
	string cmdName = strUpper( tokens[4] );

	newLevel = atoi( tokens[5].c_str() );

	if( newLevel < -2 || newLevel > 1001 )
	{
		Net->Send( "%s O %s :%d is not a valid level.\n",
				nDst.c_str(), nSrc.c_str(), newLevel );
		return CMD_ERROR;
	}

	while( PrivMsgTab[cmd].CmdName != "-" )
	{
		if( PrivMsgTab[cmd].CmdPubName == cmdName )
		{
			break;
		}

		cmd++;
	}

	if( PrivMsgTab[cmd].CmdName == "-" )
	{
		Net->Send( "%s O %s :Command %s does not exist.\n",
				nDst.c_str(), nSrc.c_str(), cmdName.c_str() );
		return CMD_ERROR;
	}

	oldLevel = PrivMsgTab[cmd].CmdLevel;
	PrivMsgTab[cmd].CmdLevel = newLevel;

	Net->Send( "%s O %s :Modified level of %s from %d to %d.\n", nDst.c_str(), nSrc.c_str(),
			cmdName.c_str(), oldLevel, newLevel );
	
	Report( CMD_CHLEVEL, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}

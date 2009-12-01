#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_CHNAME ( Numeric nSrc, Numeric nDst, Token tokens )
{
	int cmd = 0;
	string oldCmdName = strUpper( tokens[4] ), newCmdName = strUpper( tokens[5] );

	while( PrivMsgTab[cmd].CmdName != "-" )
	{
		if( PrivMsgTab[cmd].CmdPubName == newCmdName )
		{
			Net->Send( "%s O %s :A command named %s already exists.\n",
					nDst.c_str(), nSrc.c_str(), newCmdName.c_str() );
			return CMD_ERROR;
		}
		else if( PrivMsgTab[cmd].CmdPubName == oldCmdName )
		{
			break;
		}

		cmd++;
	}

	if( PrivMsgTab[cmd].CmdName == "-" )
	{
		Net->Send( "%s O %s :Command %s does not exist.\n",
				nDst.c_str(), nSrc.c_str(), oldCmdName.c_str() );
		return CMD_ERROR;
	}

	PrivMsgTab[cmd].CmdPubName = newCmdName;

	Net->Send( "%s O %s :Renamed %s to %s.\n", nDst.c_str(), nSrc.c_str(),
			oldCmdName.c_str(), newCmdName.c_str() );
	
	Report( CMD_CHNAME, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}

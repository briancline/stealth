#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_DISABLE ( Numeric nSrc, Numeric nDst, Token tokens )
{
	int i = 0;
	string cmdName = strUpper( tokens[4] );

	for( i = 0; PrivMsgTab[i].CmdPubName != "-"; i++ )
	{
		if( strCmp( cmdName, PrivMsgTab[i].CmdPubName ) )
		{
			if( PrivMsgTab[i].CmdEnabled == false )
			{
				Net->Send( "%s O %s :Command %s is already disabled.\n", nDst.c_str(),
					nSrc.c_str(), PrivMsgTab[i].CmdPubName.c_str() );
				return CMD_ERROR;
			}
			else
			{
				PrivMsgTab[i].CmdEnabled = false;
				Net->Send( "%s O %s :Command %s is now disabled.\n", nDst.c_str(),
					nSrc.c_str(), PrivMsgTab[i].CmdPubName.c_str() );
				return CMD_SUCCESS;
			}
		}
	}

	Net->Send( "%s O %s :Command %s does not exist.\n", nDst.c_str(), nSrc.c_str(),
		cmdName.c_str() );

	Report( CMD_DISABLE, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_ERROR;
}


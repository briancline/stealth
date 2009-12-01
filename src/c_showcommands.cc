#include <string>
#include <list>
#include "stealth.h"
#include "cmdmap.h"
#include "network.h"

using namespace std;

cmdStatusType cmd_SHOWCOMMANDS ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Client *user = Net->FindClientByNum( nSrc );
	int curLevel = user->GetLevel(), cmd = 0;
	string cmdString;
	list< string > cmdList;
	list< string >::iterator i;

	for( curLevel = user->GetLevel(); curLevel >= 0; curLevel-- )
	{
		cmdString.erase();
		cmdList.erase( cmdList.begin(), cmdList.end() );

		for( cmd = 0; PrivMsgTab[cmd].CmdPubName != "-"; cmd++ )
		{
			if( PrivMsgTab[cmd].CmdLevel == curLevel && PrivMsgTab[cmd].CmdEnabled == true )
			{
				cmdList.push_back( PrivMsgTab[cmd].CmdPubName );
			}
		}

		if( !cmdList.empty() )
		{
			cmdList.sort();
			for( i = cmdList.begin(); i != cmdList.end(); i++ )
			{
				cmdString.append( " " );
				cmdString.append( *i );
			}
			cmdString = strLower( cmdString );

			Net->Send( "%s O %s :\002Level %4d:\002%s\n", nDst.c_str(), nSrc.c_str(),
				curLevel, cmdString.c_str() );
		}
	}

	Report( CMD_SHOWCOMMANDS, nSrc, 0, 0 );

	return CMD_SUCCESS;
}


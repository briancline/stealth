#include <string>
#include <algorithm>
#include <stdlib.h>
#include "stealth.h"
#include "network.h"
#include "msgmap.h"
#include "cmdmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_P ( Numeric nSrc, Numeric nDst, Token params )
{
	string CmdName = params[3];
	Client *user = Net->FindClientByNum( nSrc ), *bot;
	bool foundHandler = false;
	unsigned int atPos = 0;
	int numParams = params.numTokens() - 4;
	
	assert( user != NULL );

	// Do not reply to service bots or any clients on this server
	if( getServNum( nSrc ) == Net->GetConfig( "SERVNUM" ) )
		return MSG_SUCCESS;

	bot = Net->FindClientByNum( nDst );
	if( bot == NULL )
	{
		atPos = nDst.find( "@" );
		if( atPos != string::npos )
		{
			nDst = nDst.substr( 0, atPos );
			bot = Net->FindClientByNick( nDst );

			if( bot != NULL )
			{
				nDst = bot->GetNumeric();
			}
			else
			{
				nDst = Net->GetConfig( "SERVNUM" );
				nDst.append( "AAA" );
			}
		}
	}
	
	CmdName = CmdName.substr( 1, CmdName.length() );        // remove leading colon
	CmdName = strUpper( CmdName );
	
	for( int i = 0; PrivMsgTab[i].CmdName != "-"; i++ )
	{
		if( strCmp( CmdName, PrivMsgTab[i].CmdPubName ) && PrivMsgTab[i].CmdEnabled == true )
		{
			foundHandler = true;
			if( PrivMsgTab[i].CmdLevel >= 0 && !user->IsOper() )
			{
				Net->Send( "%s O %s :You must be an oper to use this service.\n", nDst.c_str(),
					nSrc.c_str() );
				return MSG_ERROR;
			}

			if( numParams < PrivMsgTab[i].CmdParams )
			{
				Net->Send( "%s O %s :\002SYNTAX:\002 %s %s\n", nDst.c_str(), nSrc.c_str(), 
					PrivMsgTab[i].CmdPubName.c_str(), PrivMsgTab[i].CmdSyntax.c_str() );
				return MSG_ERROR;
			}

			if( PrivMsgTab[i].CmdLevel > user->GetLevel() )
			{
				Net->Send( "%s O %s :Not enough access.\n", nDst.c_str(), nSrc.c_str() );
				return MSG_ERROR;
			}

			PrivMsgTab[i].Exec( nSrc, nDst, params );
			break;
		}
	}

	if( !foundHandler )
	{
		Net->Send( "%s O %s :%s is not a valid command.\n", nDst.c_str(), nSrc.c_str(), 
			CmdName.c_str() );
		Net->Send( "%s O %s :Use SHOWCOMMANDS to see a list of all commands you have access to.\n",
			nDst.c_str(), nSrc.c_str() );
		return MSG_ERROR;
	}
	
	return MSG_SUCCESS;
}


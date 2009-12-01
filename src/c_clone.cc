#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_CLONE ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Client *user = Net->FindClientByNick( tokens[4] );
	string nick, ident, host, realName, ip, modes, num;

	if( user != NULL )
	{
		Net->Send( "%s O %s :Nick %s is already taken.\n", nDst.c_str(), nSrc.c_str(),
			user->GetNick().c_str() );

		return CMD_ERROR;
	}

	nick = tokens[4];
	ident = tokens[5];
	host = tokens[6];
	realName = tokens.Assemble( 8 );
	ip = "AAAAAA";
	modes = "+d";
	num = Net->GetNextNumeric( Net->GetConfig( "SERVNUM" ) );

	Net->AddClient( Net->GetConfig( "SERVNUM" ), nick, 1, time( NULL ), 
		ident, host, modes, ip, num, realName );
	
	Net->Send( "%s N %s 1 %d %s %s %s %s %s :%s\n",
		Net->GetConfig( "SERVNUM" ),
		nick.c_str(),
		time( NULL ),
		ident.c_str(),
		host.c_str(),
		modes.c_str(),
		ip.c_str(),
		num.c_str(),
		realName.c_str() );
	
	Report( CMD_CLONE, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


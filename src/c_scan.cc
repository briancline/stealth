#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_SCAN ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Server *serv;
	Client *user;
	ClientMapType::iterator iter;
	string mask = strLower( tokens[4] ), servName, curMask, curIPMask;
	int count = 0, dotPos = 0;

	for( iter = Net->ClientIter(); iter != Net->ClientTail(); iter++ )
	{
		user = iter->second;

		curMask = user->GetUserName();
		curMask.append( "@" );
		curMask.append( user->GetHostName() );

		curIPMask = user->GetUserName();
		curIPMask.append( "@" );
		curIPMask.append( user->GetIP() );

		if( match( mask, curMask ) || match( mask, curIPMask ) )
		{
			serv = Net->FindServerByNum( getServNum( user->GetNumeric() ) );
			dotPos = serv->GetName().find( "." );
			servName = serv->GetName().substr( 0, dotPos );
			
			Net->Send( "%s O %s :   %s!%s@%s [%s] on %s.*\n", nDst.c_str(), nSrc.c_str(),
				user->GetNick().c_str(), user->GetUserName().c_str(),
				user->GetHostName().c_str(), user->GetIP().c_str(),
				servName.c_str() );
			
			count++;
		}
	}

	Net->Send( "%s O %s :\002%d matches found\002\n", nDst.c_str(), nSrc.c_str(), count );

	Report( CMD_SCAN, nSrc, 0, tokens.Assemble( 5 ).c_str() );
	
	return CMD_SUCCESS;
}


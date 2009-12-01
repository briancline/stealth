#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"
#include "network.h"

using namespace std;


msgStatusType msg_W ( Numeric nSrc, Numeric nDst, Token params )
{
	string nick = params[3].substr( 1, params[3].length() );
	Server *serv = Net->FindServerByNum( nDst );
	Client *user = Net->FindClientByNick( nick );

	if( user == NULL )
	{
		Net->Send( "%s 401 %s %s :No such nick\n", Net->GetConfig( "SERVNUM" ), nSrc.c_str(), 
			nick.c_str() );
	}
	else
	{
		nick = user->GetNick();
		
		Net->Send( "%s 311 %s %s %s %s * :%s\n", Net->GetConfig( "SERVNUM" ), nSrc.c_str(), 
			nick.c_str(), user->GetUserName().c_str(), user->GetHostName().c_str(), 
			user->GetRealName().c_str() );
			
		Net->Send( "%s 312 %s %s %s :%s\n", Net->GetConfig( "SERVNUM" ), nSrc.c_str(), nick.c_str(), 
			serv->GetName().c_str(), Net->GetConfig( "SERVREASON" ) );

		if( user->IsOper() )
		{
			Net->Send( "%s 313 %s %s :is an IRC Operator\n", Net->GetConfig( "SERVNUM" ), 
				nSrc.c_str(), nick.c_str() );
		}
	}
	Net->Send( "%s 318 %s %s :End of /WHOIS list.\n", Net->GetConfig( "SERVNUM" ), nSrc.c_str(), 
		nick.c_str() );

	Report( SRV_WHOIS, nSrc, 0, user->GetNick().c_str(), serv->GetName().c_str() );

	return MSG_SUCCESS;
}


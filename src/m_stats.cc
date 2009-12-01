#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_R ( Numeric nSrc, Numeric nDst, Token params )
{
	// ABAAG R u :Vs
	char statChar = params[2][0];
	string servNum = params[3].substr( 1, 2 );
	Server *serv = Net->FindServerByNum( servNum );
	
	switch( statChar )
	{
		case 'u':
			Net->Send( "%s 242 %s :Server Up %s\n", servNum.c_str(), nSrc.c_str(), Net->GetUptime().c_str() );
			break;
	}

	Net->Send( "%s 219 %s %c :End of /STATS report\n", servNum.c_str(), nSrc.c_str(), statChar );

	Report( SRV_STATS, nSrc, 0, statChar, serv->GetName().c_str() );

	return MSG_SUCCESS;
}


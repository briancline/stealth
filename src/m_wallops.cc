#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_WA ( Numeric nSrc, Numeric nDst, Token params )
{
	string wallMsg = params.Assemble( 3 ), src;
	Client *user = Net->FindClientByNum( nSrc );
	Server *serv = Net->FindServerByNum( nSrc );

	if( user != NULL )
		src = user->GetNick();
	else if( serv != NULL )
		src = serv->GetName();

	wallMsg = wallMsg.substr( 1, wallMsg.length() );

	Report( SRV_WALLOPS, nSrc, 0, wallMsg.c_str() );

	return MSG_SUCCESS;
}


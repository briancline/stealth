#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"
#include "version.h"

using namespace std;


msgStatusType msg_V ( Numeric nSrc, Numeric nDst, Token params )
{
	string servNum = params[2].substr( 1, 2 );
	Server *serv = Net->FindServerByNum( servNum );

	Net->Send( "%s 351 %s stealth-%s.%s.(%s) %s :%s\n", servNum.c_str(), nSrc.c_str(), 
		VERSION_MAJOR, VERSION_PATCH, VERSION_STAGE, Net->GetConfig( "SERVNAME" ), STEALTH_URL );

	Report( SRV_VERSION, nSrc, 0, serv->GetName().c_str() );

	return MSG_SUCCESS;
}


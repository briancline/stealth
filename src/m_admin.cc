#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"
#include "version.h"

using namespace std;


msgStatusType msg_AD ( Numeric nSrc, Numeric nDst, Token params )
{
	string servNum = params[2].substr( 1, params[2].length() );
	Server *serv = Net->FindServerByNum( servNum );

	Net->Send( "%s 256 %s :Administrative info about %s\n", servNum.c_str(),
		nSrc.c_str(), serv->GetName().c_str() );
	Net->Send( "%s 257 %s :Stealth Oper Service v%s.%s (%s)\n", servNum.c_str(), nSrc.c_str(), 
		VERSION_MAJOR, VERSION_PATCH, VERSION_STAGE );
	Net->Send( "%s 258 %s :Available at %s\n", servNum.c_str(), nSrc.c_str(), 
		STEALTH_URL );
	Net->Send( "%s 259 %s :%s\n", servNum.c_str(), nSrc.c_str(), Net->GetConfig( "ADMINLINE" ) );

	Report( SRV_ADMIN, nSrc, 0, serv->GetName().c_str() );

	return MSG_SUCCESS;
}


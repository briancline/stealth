#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_JU ( Numeric nSrc, Numeric nDst, Token params )
{
	// <prefix> JU <target> (+|-)<server> <expiration> <lastmod> :<reason>

	string srvName = params[3].substr( 1, params[3].length() ), srvReason;
	bool active = false;

	if( params[3][0] == '+' )
		active = true;
	else
		active = false;

	srvReason = params.Assemble( 7 );
	if( srvReason[0] == ':' )
		srvReason = srvReason.substr( 1, srvReason.length() );
	
	Net->AddJupe( srvName, atol( params[4].c_str() ), atol( params[5].c_str() ), srvReason, active );

	Report( SRV_JUPE, nSrc, 0, srvName.c_str() );

	return MSG_SUCCESS;
}


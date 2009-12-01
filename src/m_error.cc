#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_ERROR ( Numeric nSrc, Numeric nDst, Token params )
{
	string errorMsg;
	
	errorMsg = params.Assemble( 2 );
	errorMsg = errorMsg.substr( 1, errorMsg.length() );
	
	Debug( PROG_ERR, "ERROR from uplink", "%s", errorMsg.c_str() );
	Report( SRV_ERROR, nSrc, 0, errorMsg.c_str() );

	return MSG_SUCCESS;
}


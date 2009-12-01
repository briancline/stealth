#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_DS ( Numeric nSrc, Numeric nDst, Token params )
{
	string desynchMsg = params.Assemble( 3 );
	desynchMsg = desynchMsg.substr( 1, desynchMsg.length() );

	Debug( CHAN_DEBUG, "DESYNCH", "%s", desynchMsg.c_str() );
	Report( SRV_DESYNCH, nSrc, 0, desynchMsg.c_str() );

	return MSG_SUCCESS;
}


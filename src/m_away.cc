#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_A ( Numeric nSrc, Numeric nDst, Token params )
{
	string awayMessage;
	
	if( params.numTokens() >= 3 )
	{
		awayMessage = params.Assemble( 3 );
		awayMessage = awayMessage.substr( 1, awayMessage.length() );
	}
	else
	{
		awayMessage = " ";
	}

	Report( SRV_AWAY, nSrc, 0, awayMessage.c_str() );

	return MSG_SUCCESS;
}

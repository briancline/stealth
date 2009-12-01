#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_G ( Numeric nSrc, Numeric nDst, Token params )
{
	string pingMessage = params.Assemble( 3 );
	
	Net->Send( "%s Z %s %s\n", Net->GetConfig( "SERVNUM" ), Net->GetConfig( "SERVNUM" ), 
		pingMessage.c_str() );
	
	Report( SRV_PING, nSrc, 0, 0 );

	return MSG_SUCCESS;
}


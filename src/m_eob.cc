#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_EB ( Numeric nSrc, Numeric nDst, Token params )
{
	Server *serv = Net->FindServerByNum( nSrc );

	assert( serv != NULL );
	
	if( serv->IsUplink() )
		Net->Send( "%s EA\n", Net->GetConfig( "ServNum" ) );
	
	serv->AddFlag( serverFlags::SERV_BURST );

	Report( SRV_EOB, nSrc, 0, 0 );
	
	return MSG_SUCCESS;
}


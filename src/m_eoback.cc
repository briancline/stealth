#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_EA ( Numeric nSrc, Numeric nDst, Token params )
{
	Server *serv = Net->FindServerByNum( nSrc );
	Jupe *jupe = Net->FindJupeByName( serv->GetName() );

	if( jupe != NULL )
	{
		jupe->Register();
		return MSG_SUCCESS;
	}
	else
	{
		serv->AddFlag( serverFlags::SERV_BURST_ACK );
		Net->SendGlines();
	}

	Report( SRV_EOBACK, nSrc, 0, 0 );

	return MSG_SUCCESS;
}


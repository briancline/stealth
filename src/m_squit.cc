#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"
#include "network.h"

using namespace std;


msgStatusType msg_SQ ( Numeric nSrc, Numeric nDst, Token params )
{
	Server *serv = Net->FindServerByName( nDst );

	Report( SRV_SQUIT, nSrc, 0, serv->GetName().c_str() );

	Net->RemAllClients( serv->GetNumeric() );
	Net->RemServerTree( serv->GetNumeric() );
	Net->RemServer( serv->GetNumeric() );

	return MSG_SUCCESS;
}


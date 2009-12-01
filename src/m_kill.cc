#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_D ( Numeric nSrc, Numeric nDst, Token params )
{
	Client *user = Net->FindClientByNum( nDst );
	string reason = params.Assemble( 4 );
	reason = reason.substr( 1, reason.length() );

	assert( user != NULL );

	Report( SRV_KILL, nSrc, 0, user->GetNick().c_str(), reason.c_str() );

	Net->RemClient( nDst );

	return MSG_SUCCESS;
}


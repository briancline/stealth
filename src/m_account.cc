#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_AC ( Numeric nSrc, Numeric nDst, Token params )
{
	Client *user = Net->FindClientByNum( nDst );
	string accountName = params[3];

	assert( user != NULL );

	user->SetAccount( accountName );

	Report( SRV_ACCOUNT, nSrc, 0, accountName.c_str() );

	return MSG_SUCCESS;
}


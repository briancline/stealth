#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_K ( Numeric nSrc, Numeric nDst, Token params )
{
	Channel *chan = Net->FindChannelByName( params[2] );
	Client *user = Net->FindClientByNum( params[3] );
	string reason = params.Assemble( 5 );
	reason = reason.substr( 1, reason.length() );

	assert( chan != NULL );

	chan->RemUser( user->GetNumeric() );

	Report( SRV_KICK, nSrc, 0, user->GetNick().c_str(), params[2].c_str(), reason.c_str() );

	return MSG_SUCCESS;
}


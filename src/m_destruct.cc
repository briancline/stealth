#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_DE ( Numeric nSrc, Numeric nDst, Token params )
{
	Channel *chan = Net->FindChannelByName( params[2] );

	assert( chan != NULL );

	Report( SRV_DESTRUCT, nSrc, 0, chan->GetName().c_str() );
	Net->RemChannel( chan->GetName() );

	return MSG_SUCCESS;
}


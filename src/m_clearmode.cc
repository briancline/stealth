#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_CM ( Numeric nSrc, Numeric nDst, Token params )
{
	Channel *chan = Net->FindChannelByName( params[2] );

	chan->ClearModes();

	Report( SRV_CLEARMODE, nSrc, 0, chan->GetName().c_str() );

	return MSG_SUCCESS;
}


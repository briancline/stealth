#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_L ( Numeric nSrc, Numeric nDst, Token params )
{
	Channel *chan;
	Token chanList( params[2], "," );
	int i = 0;

	for( i = 0; i < chanList.numTokens(); i++ )
	{
		chan = Net->FindChannelByName( chanList[i] );
		if( chan )
		{
			chan->RemUser( nSrc );
			Report( SRV_LEAVE, nSrc, 0, chan->GetName().c_str() );
		}
		else
		{
			Debug( PROG_ERR, "msg_L", "Could not find channel %s on network", chanList[i].c_str() );
		}
	}

	return MSG_SUCCESS;
}


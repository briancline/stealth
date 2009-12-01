#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_C ( Numeric nSrc, Numeric nDst, Token params )
{
	Channel *chan;
	Token chanList( params[2], "," );
	time_t chanTS = atol( params[3].c_str() );
	string createModes = "o";
	int i = 0;

	for( i = 0; i < chanList.numTokens(); i++ )
	{
		chan = Net->AddChannel( chanList[i], chanTS );
		chan->AddUser( nSrc, createModes );
		Report( SRV_CREATE, nSrc, 0, chan->GetName().c_str() );
	}

	return MSG_SUCCESS;
}


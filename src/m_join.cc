#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_J ( Numeric nSrc, Numeric nDst, Token params )
{
	Channel *chan;
	Token chanList;
	int i = 0;

	if( params[2] == "0" )
	{
		Client *user = Net->FindClientByNum( nSrc );
		user->RemAllChans();

		return MSG_SUCCESS;
	}

	chanList.Tokenize( params[2], "," );	
	for( i = 0; i < chanList.numTokens(); i++ )
	{
		chan = Net->FindChannelByName( chanList[i] );
		assert( chan != NULL );
		chan->AddUser( nSrc );
		Report( SRV_JOIN, nSrc, 0, chan->GetName().c_str() );
	}

	return MSG_SUCCESS;
}


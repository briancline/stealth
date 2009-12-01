#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_T ( Numeric nSrc, Numeric nDst, Token params )
{
	Channel *chan = Net->FindChannelByName( params[2] );
	int topicInd = 0;
	string topic;

	assert( chan != NULL );

	for( topicInd = 3; topicInd < params.numTokens(); topicInd++ )
	{
		if( params[topicInd][0] == ':' )
			break;
	}

	topic = params.Assemble( topicInd + 1 );
	topic = topic.substr( 1, topic.length() );

	chan->SetTopic( topic );

	Report( SRV_TOPIC, nSrc, 0, chan->GetName().c_str(), chan->GetTopic().c_str() );

	return MSG_SUCCESS;
}

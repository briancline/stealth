#include <string>
#include "stealth.h"
#include "network.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"
#include "flags.h"

using namespace std;
using namespace channelFlags;


msgStatusType msg_B ( Numeric nSrc, Numeric nDst, Token params )
{
	unsigned int len = 0;
	int chanLimit = 0, posUser = 4, posBans = 0, i = 0, b = 0, u = 0;
	bool hasLimit = false, hasKey = false, hasModes = false;
	time_t starttime = atol( params[3].c_str() );
	string modes = params[4], chanModes, chanKey, banString, userNum, userModes, strUserList;
	Token banList, userList;
	Channel *chan = Net->FindChannelByName( params[2] );

	if( params[4][0] == '+' )
	{
		hasModes = true;
		posUser = 5;
	}

	if( hasModes )
	{
		if( modes.find( 'l' ) != string::npos )
			hasLimit = true;

		if( modes.find( 'k' ) != string::npos )
			hasKey = true;

		chanModes = modes;

		if( hasLimit && hasKey )
		{
			chanLimit = atoi( params[5].c_str() );
			chanKey = params[6];
			posUser = 7;

			chanModes.append( " " );
			chanModes.append( params[5] );
			chanModes.append( " " );
			chanModes.append( chanKey );
		}
		else if( hasLimit )
		{
			chanLimit = atoi( params[5].c_str() );
			posUser = 6;

			chanModes.append( " " );
			chanModes.append( params[5] );
		}
		else if( hasKey )
		{
			chanKey = params[5];
			posUser = 6;

			chanModes.append( " " );
			chanModes.append( chanKey );
		}
	}

	if( chan != NULL )
	{
		if( starttime < chan->GetTS() )
		{
			chan->ClearOps();
			chan->ClearVoices();
			chan->SetTS( starttime );
			chan->ClearModes();

			if( hasModes )
				chan->SetModes( chanModes );

			if( strLower(chan->GetName()) == strLower(Net->GetConfig( "INFOCHAN" )))
			{
				ChannelClient *chanUsr;
				string servNum = Net->GetConfig( "SERVNUM" );
				string svcNum = servNum + "AAA";

				chanUsr = chan->FindUser( svcNum );
				if( chanUsr != NULL )
				{
					chanUsr->SetOp();
					Net->Send( "%s M %s +o %s\n", servNum.c_str(), chan->GetName().c_str(),
						svcNum.c_str() );
				}
			}
		}
	}
	else
	{
		if( hasModes )
			chan = Net->AddChannel( params[2], starttime, chanModes );
		else
			chan = Net->AddChannel( params[2], starttime );
	}
	
	for( i = 4; i < params.numTokens(); i++ )
	{
		if( params[i][0] == ':' )
		{
			posBans = i;
			break;
		}
	}
	
	if( posBans > 0 )
	{
		banString = params.Assemble( posBans + 1 );
		banString = banString.substr( 1, banString.length() );
		
		banList.Tokenize( banString );
		banList[0] = banList[0].substr( 1, banList[0].length() );
		
		for( b = 0; b < banList.numTokens(); b++ )
		{
			chan->AddBan( banList[b] );
		}
		
		strUserList = params.Assemble( posUser + 1, posBans + 1 );
	}
	else
	{
		strUserList = params.Assemble( posUser + 1 );
	}
	
	userList.Tokenize( strUserList, "," );
	
	for( u = 0; u < userList.numTokens(); u++ )
	{
		len = userList[u].length();
		userNum = userList[u].substr( 0, 5 );

		if( len > 5 )
			userModes = userList[u].substr( 6, len );
		else
			userModes.erase();
		
		chan->AddUser( userNum, userModes );
	}
	
	Report( SRV_BURST, nSrc, 0, chan->GetName().c_str(), chan->GetTS(), chan->GetModeString().c_str() );

	return MSG_SUCCESS;
}

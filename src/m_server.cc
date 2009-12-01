#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"
#include "network.h"

using namespace std;


msgStatusType msg_S ( Numeric nSrc, Numeric nDst, Token params )
{
	bool isHub = false;
	int descPos, maxConn = 0, hopCount = 0;
	long int startTime;
	string servNum, servDesc;

	/*
	 * 0  1 2                              3 4 5          6   7     8   9
	 * QA S RockHill-R.SC.US.Virtuanet.org 2 0 1006954226 P10 ABAf] +h :Winthrop University
	 */

	if( params[8][0] == '+' || params[8][0] != ':' )
	{
		isHub = true;
		descPos = 9;
	}
	else
	{
		isHub = false;
		descPos = 8;
	}

	startTime = atol( params[5].c_str() );
	hopCount = atoi( params[3].c_str() );
	servNum = params[7].substr( 0, 2 );
	maxConn = Base64ToInt( params[7].substr( 2, 3 ) );
	servDesc.erase();

	servDesc = params.Assemble( ++descPos );
	servDesc = servDesc.substr( 1, servDesc.length() );

	Net->AddServer( params[0], params[2], hopCount, startTime, servNum,
			maxConn, servDesc, false );

	Report( SRV_SERVER, nSrc, 0, params[2].c_str() );

	return MSG_SUCCESS;
}


msgStatusType msg_SERVER ( Numeric nSrc, Numeric nDst, Token params )
{
	bool isHub = false;
	int descPos, maxConn = 0, hopCount = 0;
	long int startTime;
	string servNum, servDesc;

	/*
	 * 0      1              2 3          4          5   6     7
	 * SERVER irc.bs-dee.org 1 1014265543 1015263980 J10 ABAP] :BS-DEE
	 * 
	 * 0      1                  2 3         4          5   6     7
	 * SERVER test.virtuanet.org 1 960000000 1007240010 J10 QAAP] :services test server
	 *
	 * 0      1                  2 3         4          5   6     7   8
	 * SERVER test.virtuanet.org 1 960000000 1007240010 J10 QAAP] +h :services test server
	 */

	if( params[7][0] == '+' || params[7][0] != ':' )
	{
		isHub = true;
		descPos = 8;
	}
	else
	{
		isHub = false;
		descPos = 7;
	}

	startTime = atol( params[4].c_str() );
	hopCount = atoi( params[2].c_str() );
	servNum = params[6].substr( 0, 2 );
	maxConn = Base64ToInt( params[6].substr( 2, 3 ) );
	servDesc.erase();

	servDesc = params.Assemble( ++descPos );
	servDesc = servDesc.substr( 1, servDesc.length() );

	Net->AddServer( Net->GetConfig( "ServNum" ), params[1], hopCount, startTime, servNum,
			maxConn, servDesc, true );

	return MSG_SUCCESS;
}


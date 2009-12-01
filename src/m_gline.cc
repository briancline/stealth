#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_GL ( Numeric nSrc, Numeric nDst, Token params )
{
	// Vo GL * +*@test.com 950000 :test gline[0]
	// Vo GL * -*@test.com
	string mask = params[3].substr( 1, params[3].length() ), reason, src;
	char toggle = params[3][0];
	time_t duration;
	Server *serv;
	Client *user;

	switch( toggle )
	{
		case '+':
			duration = atol( params[4].c_str() );
			reason = params.Assemble( 6 );
			reason = reason.substr( 1, reason.length() );

			if( nSrc.length() == 2 )
			{
				serv = Net->FindServerByNum( nSrc );
				src = serv->GetName();
			}
			else
			{
				user = Net->FindClientByNum( nSrc );
				src = user->GetNick();
			}

			Net->AddGline( mask, duration, reason, src );
			break;


		case '-':
			Net->RemGline( mask );
			break;
	}
	
	Report( SRV_GLINE, nSrc, 0, mask.c_str(), duration, reason.c_str() );
	
	return MSG_SUCCESS;
}


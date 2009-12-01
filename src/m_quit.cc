#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"
#include "network.h"

using namespace std;


msgStatusType msg_Q ( Numeric nSrc, Numeric nDst, Token params )
{
	Client *user = Net->FindClientByNum( nSrc );
	string reason = params.Assemble( 3 );
	reason = reason.substr( 1, reason.length() );

	if( user == NULL )
	{
		Debug( PROG_ERR, "msg_Q", "Could not find user %s", nSrc.c_str() );
		return MSG_ERROR;
	}

	/* This call to Report() needs to come BEFORE RemClient. Otherwise,
	 * Report() will not be able to find the user that the numeric (nSrc)
	 * points to, and will not show the correct nick.
	 */
	Report( SRV_QUIT, nSrc, 0, reason.c_str() );

	Net->RemClient( nSrc );

	return MSG_SUCCESS;
}


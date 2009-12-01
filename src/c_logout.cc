#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_LOGOUT ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Client *user = Net->FindClientByNum( nSrc );

	assert( user != NULL );

	if( !user->IsAuthed() )
	{
		Net->Send( "%s O %s :You are not logged in.\n", nDst.c_str(), nSrc.c_str() );
		return CMD_ERROR;
	}

	user->ClearAccess();

	Net->Send( "%s O %s :You have been deauthenticated.\n", nDst.c_str(), nSrc.c_str() );

	Report( CMD_LOGOUT, nSrc, 0, 0 );

	return CMD_SUCCESS;
}


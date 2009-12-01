#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_SETTIME ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Net->Send( "%s SE %ld :%s\n", Net->GetConfig( "SERVNUM" ), time( NULL ),
		Net->GetConfig( "SERVNAME" ) );

	Report( CMD_SETTIME, nSrc, 0, 0 );

	return CMD_SUCCESS;
}

#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_RESTART ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Client *oper = Net->FindClientByNum( nSrc );

	Net->Send( "%s WA :Restarting (%s)\n", Net->GetConfig( "SERVNUM" ),
		oper->GetAccount().c_str() );
	Net->Send( "%s SQ %s 0 :Restart by %s\n", Net->GetConfig( "SERVNUM" ),
		Net->GetConfig( "SERVNAME" ), oper->GetAccount().c_str() );
	
	connecting = true;
	
	Report( CMD_RESTART, nSrc, 0, 0 );

	return CMD_SUCCESS;
}

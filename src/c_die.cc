#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_DIE ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Client *oper = Net->FindClientByNum( nSrc );

	Net->Send( "%s WA :Quitting (%s)\n", Net->GetConfig( "SERVNUM" ),
		oper->GetAccount().c_str() );
	Net->Send( "%s SQ %s 0 :Die by %s\n", Net->GetConfig( "SERVNUM" ),
		Net->GetConfig( "SERVNAME" ), oper->GetAccount().c_str() );

	dying = true;

	Report( CMD_DIE, nSrc, 0, 0 );

	return CMD_SUCCESS;
}

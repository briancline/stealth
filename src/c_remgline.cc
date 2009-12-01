#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_REMGLINE ( Numeric nSrc, Numeric nDst, Token tokens )
{
	string hostmask = tokens[4];
	
	Net->RemGline( hostmask );
	Net->Send( "%s GL * -%s\n", Net->GetConfig( "SERVNUM" ), hostmask.c_str() );
	
	Report( CMD_REMGLINE, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


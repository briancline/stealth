#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_QUOTE ( Numeric nSrc, Numeric nDst, Token tokens )
{
	string sendText = tokens.Assemble( 5 );
	
	Net->Send( "%s\n", sendText.c_str() );

	Report( CMD_QUOTE, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}

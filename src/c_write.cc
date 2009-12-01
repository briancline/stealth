#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_WRITE ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Net->SaveData();
	Net->Send( "%s WA :Saved all files\n", Net->GetConfig("SERVNUM") );

	Report( CMD_WRITE, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


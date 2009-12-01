#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_REFRESHG ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Net->SendGlines();
	
	Report( CMD_REFRESHG, nSrc, 0, 0 );
	
	return CMD_SUCCESS;
}

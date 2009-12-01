#include "stealth.h"
#include "cmdmap.h"
#include "network.h"

using namespace std;

cmdStatusType cmd_UPTIME ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Net->Send( "%s O %s :Uptime: %s\n", nDst.c_str(), nSrc.c_str(),
		Net->GetUptime().c_str() );
	Net->Send( "%s O %s :Transfer stats: %s\n", nDst.c_str(), nSrc.c_str(),
		Net->GetSocketStats().c_str() );

	Report( CMD_UPTIME, nSrc, 0, 0 );

	return CMD_SUCCESS;	
}


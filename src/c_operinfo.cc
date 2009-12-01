#include "stealth.h"
#include "cmdmap.h"
#include "network.h"

using namespace std;

cmdStatusType cmd_OPERINFO ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Access *acc = Net->FindAccessEntry( tokens[4] );

	if( acc == NULL )
	{
		Net->Send( "%s O %s :User %s not found in access map.\n", nDst.c_str(), nSrc.c_str(), 
			tokens[4].c_str() );
		return CMD_ERROR;
	}

	Net->Send( "%s O %s :Nick:      %s\n", nDst.c_str(), nSrc.c_str(), acc->GetNick().c_str() );
	Net->Send( "%s O %s :Level:     %d\n", nDst.c_str(), nSrc.c_str(), acc->GetLevel() );
	Net->Send( "%s O %s :Suspend:   %s -- Protect: %s\n", nDst.c_str(), nSrc.c_str(),
		acc->IsSuspended() ? "YES" : "NO", 
		acc->IsProtected() ? "YES" : "NO" );
	Net->Send( "%s O %s :Hostmasks: %s\n", nDst.c_str(), nSrc.c_str(), acc->GetMasks().c_str() );
	Net->Send( "%s O %s :Last seen: %s\n", nDst.c_str(), nSrc.c_str(), acc->GetLastSeen().c_str() );

	Report( CMD_OPERINFO, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


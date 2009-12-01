#include "stealth.h"
#include "cmdmap.h"
#include "version.h"

using namespace std;

cmdStatusType cmd_CTCP_PING ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Net->Send( "%s O %s :\001PING %s\n", nDst.c_str(), nSrc.c_str(), 
		tokens[4].c_str() );
	
	Report( CMD_CTCP_PING, nSrc, 0, 0 );

	return CMD_SUCCESS;
}


cmdStatusType cmd_CTCP_VERSION ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Net->Send( "%s O %s :\001VERSION Stealth Oper Service v%s.%s (%s), available from %s\001\n", nDst.c_str(), 
		nSrc.c_str(), VERSION_MAJOR, VERSION_PATCH, VERSION_STAGE, STEALTH_URL );
	
	Report( CMD_CTCP_VERSION, nSrc, 0, 0 );

	return CMD_SUCCESS;
}


cmdStatusType cmd_CTCP_GENDER ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Net->Send( "%s O %s :\001GENDER %s\001\n", nDst.c_str(), nSrc.c_str(),
		Net->GetConfig( "GENDER" ) );
	
	Report( CMD_CTCP_GENDER, nSrc, 0, 0 );

	return CMD_SUCCESS;
}


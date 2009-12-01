#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_BROADCAST ( Numeric nSrc, Numeric nDst, Token tokens )
{
	ClientMapType::iterator clientIter = Net->ClientIter();
	string broadcastMsg;

	for( int t = 4; t < tokens.numTokens(); t++ )
	{
		if( t > 4 )
			broadcastMsg.append( " " );

		broadcastMsg.append( tokens[t] );
	}

	for( clientIter = Net->ClientIter(); clientIter != Net->ClientTail(); clientIter++ )
	{
		if( !clientIter->second->IsService() && 
			getServNum( clientIter->second->GetNumeric() ) != Net->GetConfig( "SERVNUM ") )
		{
			Net->Send( "%s O %s :\002[Global Broadcast]:\002 %s\n", nDst.c_str(), 
				clientIter->second->GetNumeric().c_str(), broadcastMsg.c_str() );
		}
	}

	Report( CMD_BROADCAST, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}

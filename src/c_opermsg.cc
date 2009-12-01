#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_OPERMSG ( Numeric nSrc, Numeric nDst, Token tokens )
{
	ClientMapType::iterator clientIter = Net->ClientIter();
	Client *user = Net->FindClientByNum( nSrc );
	string broadcastMsg;

	for( int t = 4; t < tokens.numTokens(); t++ )
	{
		if( t > 4 )
			broadcastMsg.append( " " );

		broadcastMsg.append( tokens[t] );
	}

	for( clientIter = Net->ClientIter(); clientIter != Net->ClientTail(); clientIter++ )
	{
		if( !clientIter->second->IsService() && clientIter->second->IsOper() )
		{
			Net->Send( "%s O %s :\002OPERMSG [%s]:\002 %s\n", nDst.c_str(), 
				clientIter->second->GetNumeric().c_str(), user->GetNick().c_str(),
				broadcastMsg.c_str() );
		}
	}

	Report( CMD_OPERMSG, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}

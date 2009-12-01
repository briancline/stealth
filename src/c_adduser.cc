#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_ADDUSER ( Numeric nSrc, Numeric nDst, Token tokens )
{
	AccessMapType::iterator aIter;
	Client *user = Net->FindClientByNum( nSrc );
	string nick = tokens[4], password = tokens[6], masks = tokens.Assemble( 8 );
	int level = atoi( tokens[5].c_str() );

	for( aIter = Net->AccessIter(); aIter != Net->AccessTail(); aIter++ )
	{
		if( strSCmp( aIter->second->GetNick(), nick ) )
		{
			Net->Send( "%s O %s :User %s already exists at level %d.\n", nDst.c_str(), nSrc.c_str(), 
				aIter->second->GetNick().c_str(), aIter->second->GetLevel() );
			return CMD_ERROR;
		}
	}

	if( level >= user->GetLevel() )
	{
		Net->Send( "%s O %s :You cannot add a user whose level is higher than or equal to your own.\n", 
			nDst.c_str(), nSrc.c_str() );
		Net->Send( "%s O %s :The valid range of user levels for you is %d-%d.\n", nDst.c_str(), 
			nSrc.c_str(), 1, user->GetLevel() - 1 );
		return CMD_ERROR;
	}

	Net->AddAccessEntry( nick, level, false, false, time( NULL ), password, masks );
	
	Net->Send( "%s O %s :%s successfully added at level %d.\n", nDst.c_str(), nSrc.c_str(), nick.c_str(), level );
	
	Report( CMD_ADDUSER, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}

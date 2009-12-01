#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_REMUSER ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Access *rec = Net->FindAccessEntry( tokens[4] );
	Client *user = Net->FindClientByNum( nSrc );
	int uLevel = user->GetLevel();
	string oldNick;

	if( rec == NULL )
	{
		Net->Send( "%s O %s :User not found.\n", nDst.c_str(), nSrc.c_str() );
		return CMD_ERROR;
	}

	if( uLevel <= rec->GetLevel() )
	{
		Net->Send( "%s O %s :You cannot remove a user whose level is greater than or equal to your own.\n",
			nDst.c_str(), nSrc.c_str() );
		return CMD_ERROR;
	}

	oldNick = rec->GetNick();
	Net->RemAccessEntry( rec->GetNick() );

	rec = Net->FindAccessEntry( tokens[4] );

	if( rec != NULL )
	{
		Net->Send( "%s O %s :Encountered error while trying to remove %s.\n", nDst.c_str(),
			nSrc.c_str(), tokens[4].c_str() );
		return CMD_ERROR;
	}

	Net->Send( "%s O %s :Removed user %s\n", nDst.c_str(), nSrc.c_str(), oldNick.c_str() );
	Net->SaveData();
	
	Report( CMD_REMUSER, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}

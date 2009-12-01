#include "stealth.h"
#include "cmdmap.h"
#include "access.h"
#include "network.h"

using namespace std;

cmdStatusType cmd_LOGIN ( Numeric nSrc, Numeric nDst, Token tokens )
{
	int matchCount = 0, i = 0;
	string hostList, fullHost;
	Access *acc;
	Client *user;
	Token hosts;

	user = Net->FindClientByNum( nSrc );
	assert( user != NULL );

	acc = user->GetAccessRecord();

	if( acc != NULL )
	{
		Net->Send( "%s O %s :You are already authenticated, silly.\n", nDst.c_str(), nSrc.c_str() );
		return CMD_ERROR;
	}

	acc = Net->FindAccessEntry( tokens[4] );

	if( acc == NULL )
	{
		Net->Send( "%s O %s :Invalid username!\n", nDst.c_str(), nSrc.c_str() );
		return CMD_ERROR;
	}

	if( !strSCmp( acc->GetPass(), tokens[5] ) )
	{
		Net->Send( "%s O %s :Invalid password!\n", nDst.c_str(), nSrc.c_str() );
		return CMD_ERROR;
	}

	hostList = acc->GetHostList();
	hosts.Tokenize( hostList );
	fullHost = user->GetNick() + "!" + user->GetUserName() + "@" + user->GetHostName();

	for( i = 0; i < hosts.numTokens(); i++ )
	{
		if( match( hosts[i], fullHost ) )
			matchCount++;
	}

	if( matchCount == 0 )
	{
		Net->Send( "%s O %s :Your host does not match any of the hostmasks for %s.\n", nDst.c_str(),
			nSrc.c_str(), acc->GetNick().c_str() );
		return CMD_ERROR;
	}

	if( acc->IsSuspended() == true )
	{
		Net->Send( "%s O %s :Your account has been suspended.\n", nDst.c_str(), nSrc.c_str() );
		return CMD_ERROR;
	}

	if( !user->HasAccount() )
	{
		user->SetAccount( acc->GetNick() );
		Net->Send( "%s AC %s %s\n", Net->GetConfig( "SERVNUM" ), nSrc.c_str(), strLower( acc->GetNick() ).c_str() );
	}

	Net->Send( "%s O %s :Authentication successful!\n", nDst.c_str(), nSrc.c_str() );

	Report( CMD_LOGIN, nSrc, 0, tokens.Assemble( 5, 6 ).c_str() );

	return CMD_SUCCESS;
}

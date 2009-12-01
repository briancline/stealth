#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_WHOIS ( Numeric nSrc, Numeric nDst, Token tokens )
{
	if( tokens.numTokens() < 5 )
		return CMD_SYNTAX;
	
	Client *user = Net->FindClientByNick( tokens[4] );
	string operString;

	if( user == NULL )
	{
		Net->Send( "%s O %s :Client %s not found.\n", nDst.c_str(),
			nSrc.c_str(), tokens[4].c_str() );
		return CMD_ERROR;
	}

	if( user->IsOper() )
		operString = "[OPER]";


	Net->Send( "%s O %s :Nick:       %s  (%s)\n", nDst.c_str(), nSrc.c_str(),
		user->GetNick().c_str(), user->GetNumeric().c_str() );

	if( user->HasAccount() )
		Net->Send( "%s O %s :Account:    %s\n", nDst.c_str(), nSrc.c_str(), user->GetAccount().c_str() );

	Net->Send( "%s O %s :Modes:      %s %s\n", nDst.c_str(), nSrc.c_str(), user->GetModeString().c_str(), 
		operString.c_str() );

	Net->Send( "%s O %s :Channels:   %s\n", nDst.c_str(), nSrc.c_str(), user->GetChanList().c_str() );

	Net->Send( "%s O %s :Real name:  %s\n", nDst.c_str(), nSrc.c_str(), user->GetRealName().c_str() );

	if( user->HasAccount() && user->IsHostHidden() )
	{
		Net->Send( "%s O %s :Full mask:  %s!%s@%s\n", nDst.c_str(), nSrc.c_str(),
			user->GetNick().c_str(), user->GetUserName().c_str(), user->GetHiddenHost().c_str() );
		Net->Send( "%s O %s :Real mask:  %s!%s@%s\n", nDst.c_str(), nSrc.c_str(),
			user->GetNick().c_str(), user->GetUserName().c_str(), user->GetHostName().c_str() );
	}
	else
	{
		Net->Send( "%s O %s :Full mask:  %s!%s@%s\n", nDst.c_str(), nSrc.c_str(),
			user->GetNick().c_str(), user->GetUserName().c_str(), user->GetHostName().c_str() );
	}

	Net->Send( "%s O %s :IP Address: %s (%s)\n", nDst.c_str(), nSrc.c_str(),
		user->GetIP().c_str(), user->GetIP64().c_str() );

	Net->Send( "%s O %s :Signed on:  %s\n", nDst.c_str(), nSrc.c_str(), 
		user->GetConnectTime().c_str() );

	Report( CMD_WHOIS, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


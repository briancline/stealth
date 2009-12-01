#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_CLEARCHAN ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Channel *chan = Net->FindChannelByName( tokens[4] );
	Client *oper = Net->FindClientByNum( nSrc ), *client;
	Gline *gline;
	ClientMapType::iterator iter;
	bool clearModes = false, clearOps = false;
	bool clearVoiced = false, clearBans = false, glineUsers = false;
	bool kickUsers = false;
	string flags = tokens[5], glineMask, glineReason, kickReason;
	unsigned int c = 0;

	assert( chan != NULL );
	
	for( c = 0; c < flags.length(); c++ )
	{
		if( flags[c] == 'm' )
			clearModes = true;
		else if( flags[c] == 'k' )
			kickUsers = true;
			//clearKey = true;
		else if( flags[c] == 'o' )
			clearOps = true;
		else if( flags[c] == 'v' )
			clearVoiced = true;
		else if( flags[c] == 'b' )
			clearBans = true;
		else if( flags[c] == 'g' )
			glineUsers = true;
	}
	

	if( clearOps )
		chan->DeopAll();

	if( clearVoiced )
		chan->DevoiceAll();

	if( clearBans )
		chan->ClearBans();
	
	if( clearModes )
	{
		chan->ClearModes();
		Net->Send( "%s M %s -spmtinrlk *\n", Net->GetConfig("SERVNUM"), chan->GetName().c_str() );
	}

	if( kickUsers )
	{
		for( iter = Net->ClientIter(); iter != Net->ClientTail(); iter++ )
		{
			client = iter->second;
			if( ( chan->FindUser( client->GetNumeric() ) != NULL ) && 
					!client->IsProtected() && client != oper )
			{
				kickReason = "Channel kick for " + chan->GetName();
				Net->Send( "%s K %s %s :%s\n", Net->GetConfig("SERVNUM"),
						chan->GetName().c_str(), client->GetNumeric().c_str(),
						kickReason.c_str() );
			}
		}
	}

	if( glineUsers )
	{
		for( iter = Net->ClientIter(); iter != Net->ClientTail(); iter++ )
		{
			client = iter->second;
			if( ( chan->FindUser( client->GetNumeric() ) != NULL ) && 
					!client->IsProtected() && client != oper )
			{
				//glineMask = "*" + client->GetUserName() + "@" + client->GetIP();
				glineMask = "*@" + client->GetIP();
				glineReason = "Channel g-line for " + chan->GetName();
				
				gline = new Gline( glineMask, 3600, glineReason, oper->GetAccount() );
				gline->Register();
				delete gline;
			}
		}
	}

	Report( CMD_CLEARCHAN, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


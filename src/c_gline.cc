#include <time.h>
#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_GLINE ( Numeric nSrc, Numeric nDst, Token tokens )
{
	GlineMapType::iterator gIter;
	AccessMapType::iterator aIter;
	int myLevel = 0, i = 0;
	string mask = tokens[4], hosts, hostMask, ipMask, timeZone, expireDate, reason;
	time_t maxtime = 2147483647, expire, duration, timediff;
	Token hostList;
	Client *user;
	Gline *gline;
	
	duration = atol( tokens[5].c_str() );
	expire = time( NULL ) + duration;

	if( mask.find( "!" ) != string::npos )
	{
		Net->Send( "%s O %s :Gline masks should be in the ident@host form. Nick masks are not allowed.\n", 
			nDst.c_str(), nSrc.c_str() );
		return CMD_ERROR;
	}

	if( mask.find( "@" ) == string::npos )
	{
		Net->Send( "%s O %s :Gline masks should be in the ident@host form.\n", nDst.c_str(),
			nSrc.c_str() );
		return CMD_ERROR;
	}

	if( expire >= maxtime || expire < 0 )
	{
		timediff = (maxtime - time( NULL )) - 120;
		Net->Send( "%s O %s :Gline duration cannot currently exceed %ld seconds.\n", nDst.c_str(),
			nSrc.c_str(), timediff );
		return CMD_ERROR;
	}

	user = Net->FindClientByNum( nSrc.c_str() );
	myLevel = user->GetLevel();
	for( aIter = Net->AccessIter(); aIter != Net->AccessTail(); aIter++ )
	{
		if( aIter->second->GetLevel() >= myLevel )
		{
			hosts = aIter->second->GetHostList();
			hostList.Tokenize( hosts );

			for( i = 0; i < hostList.numTokens(); i++ )
			{
				if( match( mask, hostList[i] ) )
				{
					Net->Send( "%s O %s :You cannot gline an oper with higher or equal access.\n",
						nDst.c_str(), nSrc.c_str() );
					Net->Send( "%s O %s :Your gline matches the hostmask of admin %s at level %d.\n",
						nDst.c_str(), nSrc.c_str(), aIter->second->GetNick().c_str(), 
						aIter->second->GetLevel() );
					return CMD_ERROR;
				}
			}
		}
	}

	for( gIter = Net->GlineIter(); gIter != Net->GlineTail(); gIter++ )
	{
		if( match( mask, gIter->second->GetHost() ) || match( gIter->second->GetHost(), mask ) )
		{
			Net->Send( "%s O %s :A matching gline (%s) already exists.\n", nDst.c_str(), nSrc.c_str(),
				gIter->second->GetHost().c_str() );
			return CMD_ERROR;
		}
	}

	reason = tokens.Assemble( 7 );

	gline = Net->AddGline( mask, duration, reason, user->GetNick() );
	gline->Register();
	
	Report( CMD_GLINE, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


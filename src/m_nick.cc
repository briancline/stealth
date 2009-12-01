#include <string>
#include <stdlib.h>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "network.h"
#include "client.h"

using namespace std;


msgStatusType msg_N ( Numeric nSrc, Numeric nDst, Token params )
{
	bool newNick = false, hasModes = false, hasAccount = false, isOper = false;
	time_t startTime, duration;
	int hopCount, modePos = 0, ipPos = 0, numPos = 0, namePos = 0, accountPos = 0;
	int cloneCount = 0, minClones = 0, maxClones = 0;
	string modes, realName, oldNick, num, account, ip, fullMask, ipMask, hostMask;
	GlineMapType::iterator gIter;
	ClientMapType::iterator cIter;
	AccessMapType::iterator aIter;
	Client *user;
	Server *serv;
	Gline *gline;
	Access *record;


	if( params.numTokens() > 4 )
	{
		newNick = true;

		if( params[7][0] == '+' ) 
			hasModes = true;
	}
	
	if( newNick == false ) 
	{
		user = Net->FindClientByNum( nSrc );
		assert( user != NULL );

		Report( SRV_NICKCHG, nSrc, 0, user->GetNick().c_str(), params[2].c_str() );

		user->SetNick( params[2] );
		return MSG_SUCCESS;
	}
	else 
	{
		/*
		 * 0  1 2     3 4          5     6            7      8      9
		 * AB N Trask 2 1007240205 trask orion.bc.net DAqAAF ABAAM :Trask
		 *
		 * 0  1 2     3 4          5     6            7     8      9      10
		 * AB N Trask 2 1007239804 trask orion.bc.net +oiwg DAqAAF ABAAL :Trask
		 *
		 * 0  1 2     3 4          5     6            7      8     9      10     11
		 * AB N Trask 2 1007239804 trask orion.bc.net +oiwgr trask DAqAAF ABAAL :Trask
		 */

		serv = Net->FindServerByNum( nSrc );
		assert( serv != NULL );
		
		hopCount = atoi( params[3].c_str() );
		startTime = atol( params[4].c_str() );

		if( hasModes )
		{
			modePos = 7;
			ipPos = 8;
			numPos = 9;
			namePos = 10;

			if( params[7].find('r') != string::npos )
				hasAccount = true;
			else
				hasAccount = false;

			if( hasAccount )
			{
				accountPos = 8;
				ipPos = 9;
				numPos = 10;
				namePos = 11;
			}
		}
		else
		{
			modePos = 0;
			ipPos = 7;
			numPos = 8;
			namePos = 9;
		}
		
		modes = params[modePos];
		ip = params[ipPos];
		num = params[numPos];
		
		realName = params.Assemble( namePos+1 );
		realName = realName.substr( 1, realName.length() );

		user = Net->AddClient( params[0], params[2], hopCount, startTime,
		                params[5], params[6], modes, ip, num, realName );

		assert( user != NULL );

		if( hasAccount )
			user->SetAccount( params[accountPos] );

		fullMask = user->GetUserName() + "@" + user->GetHostName();
		ipMask = user->GetUserName() + "@" + user->GetIP();

		
		if( serv->CheckFlag( serverFlags::SERV_BURST_ACK ) )
		{
			for( gIter = Net->GlineIter(); gIter != Net->GlineTail(); gIter++ )
			{
				gline = gIter->second;
				if( match( fullMask, gline->GetHost() ) || match( ipMask, gline->GetHost() ) ||  
					 match( gline->GetHost(), fullMask ) || match( gline->GetHost(), ipMask ) )
				{
					gline->Register();
				}
			}
		}

		maxClones = atoi( Net->GetConfig( "CLONEGLINE" ) );
		minClones = atoi( Net->GetConfig( "CLONEWARN" ) );

		if( maxClones > 0 )
		{
			for( cIter = Net->ClientIter(); cIter != Net->ClientTail(); cIter++ )
			{
				if( strSCmp( ip, "AAAAAA" ) || user->IsService() )
					continue;

				if( cIter->second == NULL )
				{
					Debug( PROG_ERR, "msg_N", "WARNING: Found NULL client! (%s)", cIter->first.c_str() );
					continue;
				}

				if( strSCmp( cIter->second->GetIP64(), ip ) )
					cloneCount++;
			}

			if( cloneCount >= maxClones )
			{
				ipMask = user->GetNick() + "!" + user->GetUserName() + "@" + user->GetIP();
				hostMask = user->GetNick() + "!" + user->GetUserName() + "@" + user->GetHostName();

				for( aIter = Net->AccessIter(); aIter != Net->AccessTail(); aIter++ )
				{
					record = aIter->second;

					if( record->IsValidHost( ipMask ) || record->IsValidHost( hostMask ) )
					{
						isOper = true;
						break;
					}
				}

				if( !isOper )
				{
					ipMask = "*@" + Base64ToIP( ip );
					duration = atol( Net->GetConfig( "CLONETIME" ) );

					gline = Net->AddGline( ipMask, duration, "Excessive connections", 
						Net->GetConfig( "SERVNUM" ) );
					gline->Register();
				}
				else
				{
					Net->Send( "%s WA :Detected %s clones from %s (not glining oper %s)\n",
						Net->GetConfig( "SERVNUM" ), cloneCount, user->GetHostName().c_str(),
						record->GetNick().c_str() );
				}
			}
			else if( cloneCount >= minClones )
			{
				Net->Send( "%s WA :Detected %d clones from %s\n", Net->GetConfig( "SERVNUM" ),
					cloneCount, Base64ToIP( ip ).c_str() );
			}
		}
	}

	serv->NumericSet( user->GetNumeric() );

	Report( SRV_NICK, nSrc, 0, user->GetNick().c_str(), user->GetUserName().c_str(),
		user->GetHostName().c_str(), user->GetRealName().c_str() );

	return MSG_SUCCESS;
}


#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_SHOW ( Numeric nSrc, Numeric nDst, Token tokens )
{
	string option = strUpper( tokens[4] ), suspended, status;
	int count = 0, totalCount = 0, totalUsers = 0, capacity = 0, totalCapacity = 0, i = 0;
	int level = 0, maxLevel = 0, clientCount = 0, jupeCount = 0, serverCount = 0;
	int glineCount = 0, accessCount = 0, channelCount = 0;
	double load = 0.0, netLoad = 0.0, totalLoad = 0.0;
	bool isService = false;
	map<string, int> ipMap;

	ClientMapType::iterator cIter, clIter;
	JupeMapType::iterator jIter;
	ServerMapType::iterator sIter;
	GlineMapType::iterator gIter;
	AccessMapType::iterator aIter;
	ChannelMapType::iterator chIter;
	map<string, int>::iterator ipMapIter;
	Client *userPtr, *cuserPtr;
	Jupe *jupePtr;
	Server *servPtr;
	Gline *glinePtr;
	Access *accessPtr;
	
	
	if( option == "OPERS" )
	{
		count = 0;
		for( cIter = Net->ClientIter(); cIter != Net->ClientTail(); cIter++ )
		{
			userPtr = cIter->second;
			if( userPtr->IsOper() )
			{
				count++;
				Net->Send( "%s O %s :%3d) %s!%s@%s\n", nDst.c_str(), nSrc.c_str(), count,
					userPtr->GetNick().c_str(),
					userPtr->GetUserName().c_str(),
					userPtr->GetHostName().c_str() );
			}
		}
	}
	else if( option == "JUPES" )
	{
		count = 0;
		for( jIter = Net->JupeIter(); jIter != Net->JupeTail(); jIter++ )
		{
			jupePtr = jIter->second;
			if( jupePtr == NULL )
				continue;

			if( jupePtr->IsActive() )
				status = "ACTIVE";
			else
				status = "INACTIVE";

			count++;
			Net->Send( "%s O %s :%3d) \002Server:\002 %s  \002Admin:\002 %s\n", nDst.c_str(), nSrc.c_str(), count,
				jupePtr->GetName().c_str(), jupePtr->GetAdmin().c_str() );
			Net->Send( "%s O %s :     \002Status:\002 %s  \002Expires:\002 %s\n", nDst.c_str(), nSrc.c_str(),
				status.c_str(), jupePtr->GetExpireString().c_str() );
			Net->Send( "%s O %s :     \002Reason:\002 %s\n", nDst.c_str(), nSrc.c_str(),
				jupePtr->GetReason().c_str() );
		}

		if( count == 0 )
			Net->Send( "%s O %s :No jupes found.\n", nDst.c_str(), nSrc.c_str() );
	}
	else if( option == "SERVERS" )
	{
		count = 0;
		for( sIter = Net->ServIter(); sIter != Net->ServTail(); sIter++ )
		{
			count++;
			servPtr = sIter->second;
			Net->Send( "%s O %s :%3d) [%s/%4d] %s\n", nDst.c_str(), nSrc.c_str(), count,
				servPtr->GetNumeric().c_str(), Base64ToInt( servPtr->GetNumeric() ),
				servPtr->GetName().c_str() );
		}
	}
	else if( option == "GLINES" )
	{
		count = 0;
		for( gIter = Net->GlineIter(); gIter != Net->GlineTail(); gIter++ )
		{
			count++;
			glinePtr = gIter->second;
			Net->Send( "%s O %s :%3d) %s (affected %d hosts)\n", nDst.c_str(), nSrc.c_str(), count, 
				glinePtr->GetHost().c_str(), glinePtr->GetNumHosts() );
			Net->Send( "%s O %s :     \002Set:\002 %s  \002Expires:\002 %s\n", nDst.c_str(), nSrc.c_str(),
				glinePtr->GetSetTime().c_str(), glinePtr->GetExpireTime().c_str() );
			Net->Send( "%s O %s :     \002Oper:\002 %s  \002Reason:\002 %s\n", nDst.c_str(), nSrc.c_str(),
				glinePtr->GetOper().c_str(), glinePtr->GetReason().c_str() );
		}
	}
	else if( option == "USERLIST" )
	{
		// Find the highest user level, and set the level to that number.
		count = 0;
		maxLevel = 0;
		
		for( aIter = Net->AccessIter(); aIter != Net->AccessTail(); aIter++ )
		{
			accessPtr = aIter->second;
			if( accessPtr->GetLevel() > maxLevel )
				maxLevel = accessPtr->GetLevel();
		}

		for( level = maxLevel; level >= 0; level-- )
		{
			for( aIter = Net->AccessIter(); aIter != Net->AccessTail(); aIter++ )
			{
				accessPtr = aIter->second;

				if( accessPtr->GetLevel() == level )
				{
					count++;

					if( accessPtr->IsSuspended() )
						suspended = "YES";
					else
						suspended = "NO";

					Net->Send( "%s O %s :%3d) Nick:  %s   Last seen: %s\n", nDst.c_str(), nSrc.c_str(), count, 
						accessPtr->GetNick().c_str(), accessPtr->GetLastSeen().c_str() );
					Net->Send( "%s O %s :     Level: %d   Suspended: %s\n", nDst.c_str(), nSrc.c_str(),
						accessPtr->GetLevel(), suspended.c_str() );
				}
			}
		}
	}
	else if( option == "CLONES" )
	{
		totalCount = 0;
		for( cIter = Net->ClientIter(); cIter != Net->ClientTail(); cIter++ )
		{
			count = 1;
			userPtr = cIter->second;
			
			for( clIter = Net->ClientIter(); clIter != Net->ClientTail(); clIter++ )
			{
				cuserPtr = clIter->second;

				if( userPtr->GetIP() == cuserPtr->GetIP() && userPtr != cuserPtr )
					count++;
			}
			if( count > 1 )
				ipMap[ userPtr->GetIP() ] = count;
		}
		for( ipMapIter = ipMap.begin(); ipMapIter != ipMap.end(); ipMapIter++ )
		{
			if( ipMapIter->second > 1 )
			{
				Net->Send( "%s O %s :    %2d clones - *@%s\n", nDst.c_str(), nSrc.c_str(),
					ipMapIter->second, ipMapIter->first.c_str() );
				totalCount++;
			}
		}
		Net->Send( "%s O %s :%d sets of clones found\n", nDst.c_str(), nSrc.c_str(), totalCount );
	}
	else if( option == "LOAD" )
	{
		for( cIter = Net->ClientIter(); cIter != Net->ClientTail(); cIter++ )
		{
			if( !cIter->second->IsService() )
				totalUsers++;
		}

		for( sIter = Net->ServIter(); sIter != Net->ServTail(); sIter++ )
		{
			count = 0;
			isService = false;
			servPtr = sIter->second;
			
			for( cIter = Net->ClientIter(); cIter != Net->ClientTail(); cIter++ )
			{
				if( strSCmp( getServNum( cIter->second->GetNumeric() ), servPtr->GetNumeric() ) )
				{
					if( cIter->second->IsService() )
					{
						isService = true;
						break;
					}
					count++;
				}
			}

			if( isService )
				continue;

			capacity = servPtr->GetMaxConn();
			totalCapacity += capacity;

			load = ( (double)count / (double)capacity ) * 100.0;
			netLoad = ( (double)count / (double)totalUsers ) * 100.0;

			Net->Send( "%s O %s :  Server %s:\n", nDst.c_str(), nSrc.c_str(), servPtr->GetName().c_str() );
			Net->Send( "%s O %s :    %d of %d total users, %0.2f%% of network\n",
					nDst.c_str(), nSrc.c_str(), count, totalUsers, netLoad );
			Net->Send( "%s O %s :    %d of %d possible users, running at %0.2f%% capacity\n", 
					nDst.c_str(), nSrc.c_str(), count, capacity, load );
			Net->Send( "%s O %s : \n", nDst.c_str(), nSrc.c_str() );
		}

		totalLoad = ( (double)totalUsers / (double)totalCapacity ) * 100.0;

		Net->Send( "%s O %s :  Network statistics:\n", nDst.c_str(), nSrc.c_str() );
		Net->Send( "%s O %s :    Capacity:     %d possible clients\n", nDst.c_str(), nSrc.c_str(), totalCapacity );
		Net->Send( "%s O %s :    Usage:        %d clients\n", nDst.c_str(), nSrc.c_str(), totalUsers );
		Net->Send( "%s O %s :    Load:         %0.2f%%\n", nDst.c_str(), nSrc.c_str(), totalLoad );
	}
	else if( option == "DISABLED" )
	{
		Net->Send( "%s O %s :All disabled commands:\n", nDst.c_str(), nSrc.c_str() );
		Net->Send( "%s O %s :   Level   Command\n", nDst.c_str(), nSrc.c_str() );
		Net->Send( "%s O %s :  ------------------------\n", nDst.c_str(), nSrc.c_str() );
		
		for( i = 0; PrivMsgTab[i].CmdPubName != "-"; i++ )
		{
			if( PrivMsgTab[i].CmdEnabled == false )
			{
				Net->Send( "%s O %s :     %3d   %s\n", nDst.c_str(), nSrc.c_str(), PrivMsgTab[i].CmdLevel, 
					PrivMsgTab[i].CmdPubName.c_str() );
				count++;
			}
		}
		Net->Send( "%s O %s :Found a total of %d disabled commands.\n", nDst.c_str(), nSrc.c_str(), count );
	}
	else if( option == "STATS" )
	{
		for( cIter = Net->ClientIter(); cIter != Net->ClientTail(); cIter++ )
			clientCount++;
		for( sIter = Net->ServIter(); sIter != Net->ServTail(); sIter++ )
			serverCount++;
		for( jIter = Net->JupeIter(); jIter != Net->JupeTail(); jIter++ )
			jupeCount++;
		for( aIter = Net->AccessIter(); aIter != Net->AccessTail(); aIter++ )
			accessCount++;
		for( gIter = Net->GlineIter(); gIter != Net->GlineTail(); gIter++ )
			glineCount++;
		for( chIter = Net->ChannelIter(); chIter != Net->ChannelTail(); chIter++ )
			channelCount++;

		Net->Send( "%s O %s :Map statistics:\n", nDst.c_str(), nSrc.c_str() );
		Net->Send( "%s O %s :  Clients:      %3d\n", nDst.c_str(), nSrc.c_str(), clientCount );
		Net->Send( "%s O %s :  Servers:      %3d\n", nDst.c_str(), nSrc.c_str(), serverCount );
		Net->Send( "%s O %s :  Channels:     %3d\n", nDst.c_str(), nSrc.c_str(), channelCount );
		Net->Send( "%s O %s :  G-lines:      %3d\n", nDst.c_str(), nSrc.c_str(), glineCount );
		Net->Send( "%s O %s :  Jupes:        %3d\n", nDst.c_str(), nSrc.c_str(), jupeCount );
		Net->Send( "%s O %s :  Access recs:  %3d\n", nDst.c_str(), nSrc.c_str(), accessCount );
	}
	else
	{
		Net->Send( "%s O %s :Invalid option \"%s\"\n", nDst.c_str(), nSrc.c_str(), option.c_str() );
		return CMD_ERROR;
	}
	
	Report( CMD_SHOW, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}

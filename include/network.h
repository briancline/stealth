#ifndef __NETWORK_H__
#define __NETWORK_H__ "$Id: network.h,v 1.28 2002/04/16 15:04:45 bcline Exp $"

#include <string>
#include <map>
#include "types.h"
#include "token.h"
#include "client.h"
#include "server.h"
#include "channel.h"
#include "socket.h"
#include "config.h"
#include "gline.h"
#include "jupe.h"
#include "timer.h"

typedef map<string, Server*, less<string> > ServerMapType;
typedef map<string, Client*, less<string> > ClientMapType;
typedef map<string, Channel*, less<string> > ChannelMapType;
typedef map<string, Access*, less<string> > AccessMapType;
typedef map<string, Gline*, less<string> > GlineMapType;
typedef map<string, Jupe*, less<string> > JupeMapType;


class Network
{
	public:
		
		Network();
		~Network();

		void Register();
		void RegisterSelf();

		Server *AddServer( string uplinkNum, string name, int hops, time_t startTime,
		                   string num, int maxConn, string desc, bool uplink );
		
		Client *AddClient( string ownerNum, string nick, int hops, time_t startTime,
		                   string ident, string host, string modes, string ip, string num,
		                   string realName );
		
		Channel *AddChannel( string channelName, time_t createTime, string modes );
		Channel *AddChannel( string channelName, time_t createTime );
		
		
		void RemServer( string serverNumeric );
		void RemClient( string userNumeric );
		void RemChannel( string channelName );
		
		void RemServerTree( string serverNumeric );
		void RemAllClients( string serverNumeric );
		
		Server *FindServerByNum( string serverNumeric );
		Server *FindServerByName( string serverName );
		
		Client *FindClientByNum( string clientNumeric );
		Client *FindClientByNick( string clientNick );
		
		Channel *FindChannelByName( string channelName );
		
		
		int LoadConfig();
		int LoadConfig( string fileName );
		char *GetConfig( string parameterName );
		void SaveData();
		void SetParameter( string parameterName, string newValue );

		int Connect();
		int Send( char *format, ... );

		void Main();
		int ParseBuffer( string strBuffer );
		int ParseServMsg( Token params );
		int ParsePrivMsg( Token params );


		ServerMapType::iterator ServIter()
			{ return serverMap.begin(); }
		ClientMapType::iterator ClientIter()
			{ return clientMap.begin(); }
		ChannelMapType::iterator ChannelIter()
			{ return channelMap.begin(); }
		AccessMapType::iterator AccessIter()
			{ return accessMap.begin(); }
		GlineMapType::iterator GlineIter()
			{ return glineMap.begin(); }
		JupeMapType::iterator JupeIter()
			{ return jupeMap.begin(); }

		ServerMapType::iterator ServTail()
			{ return serverMap.end(); }
		ClientMapType::iterator ClientTail()
			{ return clientMap.end(); }
		ChannelMapType::iterator ChannelTail()
			{ return channelMap.end(); }
		AccessMapType::iterator AccessTail()
			{ return accessMap.end(); }
		GlineMapType::iterator GlineTail()
			{ return glineMap.end(); }
		JupeMapType::iterator JupeTail()
			{ return jupeMap.end(); }


		Access *AddAccessEntry( string usrNick, int usrLevel, bool usrProtect,
			bool usrSuspend, time_t usrLastSeen, string usrPasswd, string usrMasks );
		void RemAccessEntry( string usrNick );
		Access *FindAccessEntry( string usrNick );

		Gline *AddGline( string mask, time_t duration, string reason, string oper );
		Gline *FindGlineByMask( string mask );
		void RemGline( string mask );
		void SendGlines();

		Jupe *AddJupe( string srvName, time_t srvDuration, string srvDesc, bool active );
		Jupe *AddJupe( string srvName, time_t srvDuration, string srvDesc, string srvAdmin, bool active );
		Jupe *AddJupe( string srvName, time_t srvDuration, time_t srvStart, string srvDesc, bool active );
		Jupe *AddJupe( string srvName, time_t srvDuration, time_t srvStart, string srvDesc, string srvAdmin, bool active );
		Jupe *FindJupeByName( string srvName );
		Jupe *FindJupeByNum( string srvNum );
		void RemJupe( string srvNum );
		void RegisterJupes();
		void RegisterJupesWithBurst();

		string GetUptime();
		string GetSocketStats();

		string GetNextNumeric( string ownerNum );

		bool FinishedBurst();


	private:

		time_t starttime;

		ClientMapType clientMap;
		ServerMapType serverMap;
		ChannelMapType channelMap;
		AccessMapType accessMap;
		GlineMapType glineMap;
		JupeMapType jupeMap;

		Socket *sock;
		Config *myConfig;

};


#endif // __NETWORK_H__

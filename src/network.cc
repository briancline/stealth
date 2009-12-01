#include <string>
#include <map>
#include <sstream>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include "stealth.h"
#include "network.h"
#include "server.h"
#include "client.h"
#include "channel.h"
#include "flags.h"
#include "socket.h"
#include "config.h"
#include "jupe.h"
#include "msgmap.h"


Network::Network()
{
	starttime = time( NULL );
	this->sock = new Socket;
}


Network::~Network()
{
	clientMap.clear();
	serverMap.clear();
	channelMap.clear();
	accessMap.clear();
	glineMap.clear();
	jupeMap.clear();
	delete sock;
	delete myConfig;
}


void Network::Register()
{
	ClientMapType::iterator clientIter;
	ChannelMapType::iterator chanIter;
	Client *client;
	Channel *chan;

	Send( "PASS :%s\n", myConfig->GetParam( "SERVPASS" ).c_str() );

	Send( "SERVER %s 1 %ld %ld J10 %s]]] +hs :%s\n",
		myConfig->GetParam( "SERVNAME" ).c_str(),
		time( NULL ),
		time( NULL ),
		myConfig->GetParam( "SERVNUM" ).c_str(),
		myConfig->GetParam( "SERVREASON" ).c_str() );
	
	this->RegisterJupes();


	for( clientIter = clientMap.begin();
			clientIter != clientMap.end();
			clientIter ++ )
	{
		client = clientIter->second;

		if( client->GetModes() > clientFlags::USER_NONE )
		{
			Send( "%s N %s 1 %d %s %s %s %s %s :%s\n",
				getServNum( client->GetNumeric() ).c_str(),
				client->GetNick().c_str(),
				client->GetConnectTS(),
				client->GetUserName().c_str(),
				client->GetHostName().c_str(),
				client->GetModeString().c_str(),
				client->GetIP64().c_str(),
				client->GetNumeric().c_str(),
				client->GetRealName().c_str() );
		}
		else
		{
			Send( "%s N %s 1 %d %s %s %s %s :%s\n",
				getServNum( client->GetNumeric() ).c_str(),
				client->GetNick().c_str(),
				client->GetConnectTS(),
				client->GetUserName().c_str(),
				client->GetHostName().c_str(),
				client->GetIP64().c_str(),
				client->GetNumeric().c_str(),
				client->GetRealName().c_str() );
		}
	}


	for( chanIter = channelMap.begin();
			chanIter != channelMap.end();
			chanIter++ )
	{
		chan = chanIter->second;

		if( chan->GetModes() > channelFlags::CHAN_NONE )
		{
			Send( "%s B %s %ld %s %s\n",
				myConfig->GetParam( "SERVNUM" ).c_str(),
				chan->GetName().c_str(),
				chan->GetTS(),
				chan->GetModeString().c_str(),
				chan->GetNumericsString().c_str() );
		}
		else
		{
			Send( "%s B %s %ld %s\n",
				myConfig->GetParam( "SERVNUM" ).c_str(),
				chan->GetName().c_str(),
				chan->GetTS(),
				chan->GetNumericsString().c_str() );
		}
	}
		
	Send( "%s EB\n", myConfig->GetParam( "SERVNUM" ).c_str() );
}


void Network::RegisterSelf()
{
	string clientIP64 = "AAAAAA", clientNumeric, protoModes, debugModes;
	Client *self;
	Channel *infoChan;

	clientNumeric = myConfig->GetParam( "SERVNUM" );
	clientNumeric.append( "AAA" );

	AddServer( myConfig->GetParam( "SERVNUM" ), myConfig->GetParam( "SERVNAME" ), 0, time( NULL ),
		myConfig->GetParam( "SERVNUM" ), 262144, myConfig->GetParam( "SERVREASON" ), false );
	
	self = AddClient( myConfig->GetParam( "SERVNUM" ), myConfig->GetParam( "CLIENTNICK" ), 0, 
		atol( myConfig->GetParam( "CLIENTTS" ).c_str() ), myConfig->GetParam( "CLIENTIDENT" ),
		myConfig->GetParam( "CLIENTHOST" ), myConfig->GetParam( "CLIENTMODES" ),
		clientIP64, clientNumeric, myConfig->GetParam( "CLIENTREASON" ) );

	infoChan = AddChannel( myConfig->GetParam( "INFOCHAN" ), time( NULL ), myConfig->GetParam( "INFOMODES" ) );
	infoChan->AddUser( self->GetNumeric(), "o" );
}


Server *Network::AddServer( string uplinkNum, string name, int hops, time_t startTime, string num, 
		int maxConn, string desc, bool uplink )
{
	serverMap[num] = new Server( uplinkNum, name, hops, startTime, num, maxConn, desc, uplink );

	return serverMap[num];
}


Client *Network::AddClient( string ownerNum, string nick, int hops, time_t startTime, string ident, 
		string host, string modes, string ip, string num, string realName )
{
	Server *serv = this->FindServerByNum( ownerNum );
	assert( serv != NULL );

	if( clientMap[ num ] != NULL )
	{
		Debug( PROG_ERR, "Network::AddClient", "Nick collision for %s/%s", nick.c_str(), num.c_str() );
		RemClient( num );
	}

	clientMap[num] = new Client( ownerNum, nick, hops, startTime, ident, host, modes,
		ip, num, realName );

	serv->NumericInc();

	return clientMap[num];
}


Channel *Network::AddChannel( string channelName, time_t createTime, string modes )
{
	Channel *chan;
	if( channelMap[channelName] != NULL )
	{
		chan = FindChannelByName( channelName );
		chan->SetTS( createTime );
		chan->SetModes( modes );
	}
	else
	{
		channelMap[channelName] = new Channel( channelName, createTime, modes );
	}

	return channelMap[channelName];
}


Channel *Network::AddChannel( string channelName, time_t createTime )
{
	Channel *chan;
	if( channelMap[channelName] != NULL )
	{
		chan = FindChannelByName( channelName );
		chan->SetTS( createTime );
	}
	else
	{
		channelMap[channelName] = new Channel( channelName, createTime );
	}

	return channelMap[channelName];
}


void Network::RemServer( string serverNumeric )
{
	serverMap.erase( serverNumeric );
}


void Network::RemClient( string userNumeric )
{
	Client *user = FindClientByNum( userNumeric );

	user->RemAllChans();
	
	clientMap.erase( userNumeric );
}


void Network::RemChannel( string channelName )
{
	channelMap.erase( channelName );
}


void Network::RemServerTree( string serverNumeric )
{
	ServerMapType::iterator current = serverMap.begin();

	for( current = serverMap.begin();
		  current != serverMap.end();
		  current++ )
	{
		if( strSCmp( current->second->GetUplinkNumeric(), serverNumeric ) )
		{
			this->RemAllClients( current->second->GetNumeric() );
			this->RemServerTree( current->second->GetNumeric() );
			serverMap.erase( current );
		}
	}
}


void Network::RemAllClients( string serverNumeric )
{
	ClientMapType::iterator current;

	for( current = clientMap.begin();
		  current != clientMap.end();
		  current++ )
	{
		if( strSCmp( getServNum( current->second->GetNumeric() ), serverNumeric ) )
		{
			clientMap.erase( current );
		}
	}
}


Server *Network::FindServerByNum( string serverNumeric )
{
	ServerMapType::const_iterator serv;

	serv = serverMap.find( serverNumeric );

	if( serv == serverMap.end() )
		return NULL;
	
	return serv->second;
}


Server *Network::FindServerByName( string serverName )
{
	ServerMapType::iterator current = serverMap.begin();

	for( current = serverMap.begin(); 
			current != serverMap.end(); 
			current++ ) 
	{
		if( strCmp( current->second->GetName(), serverName ) )
			return current->second;
	}
	return NULL;
}


Client *Network::FindClientByNum( string clientNumeric )
{
	ClientMapType::const_iterator user;

	user = clientMap.find( clientNumeric );

	if( user == clientMap.end() )
		return NULL;

	return user->second;
}


Client *Network::FindClientByNick( string clientNick )
{
	for( ClientMapType::iterator current = clientMap.begin();
			current != clientMap.end(); 
			current++ )
	{
		if( strCmp( current->second->GetNick(), clientNick ) )
			return current->second;
	}
	return NULL;
}


Channel *Network::FindChannelByName( string channelName )
{
	for( ChannelMapType::iterator current = channelMap.begin();
			current != channelMap.end();
			current++ )
	{
		if( strCmp( current->second->GetName(), channelName ) )
			return current->second;
	}
	return NULL;
}


int Network::LoadConfig()
{
	myConfig = new Config;
	myConfig->LoadConfiguration();
	this->RegisterSelf();
	myConfig->LoadCommands();
	myConfig->LoadUsers();
	myConfig->LoadJupes();
	myConfig->LoadClones();
	myConfig->LoadChans();
	return 0;
}


int Network::LoadConfig( string fileName )
{
	myConfig = new Config( fileName );
	myConfig->LoadConfiguration();
	this->RegisterSelf();
	myConfig->LoadCommands();
	myConfig->LoadUsers();
	myConfig->LoadJupes();
	myConfig->LoadClones();
	myConfig->LoadChans();
	return 0;
}


char *Network::GetConfig( string parameterName )
{
	char *parameterVal;

	parameterName = strUpper( parameterName );
	parameterVal = (char*)myConfig->GetParam( parameterName ).c_str();

	return parameterVal;
}


void Network::SaveData()
{
	myConfig->SaveUsers();
	myConfig->SaveJupes();
	myConfig->SaveClones();
	myConfig->SaveChans();
	myConfig->SaveCommands();
}


void Network::SetParameter( string parameterName, string newValue )
{
	myConfig->SetParam( parameterName, newValue );
}


void Network::Main()
{
	char *buffer = new char[1024];
	time_t currentTS, nextTimer, nextRun;
	unsigned int t = 0;
	int fdStatus = 0, sockFD = sock->GetFD();
	fd_set readSet, writeSet;
	Timer *timer;
	timeval tv;

	while( 1 )
	{
		if( connecting || dying )
			break;

		FD_ZERO( &readSet );
		FD_ZERO( &writeSet );
		FD_SET( sockFD, &readSet );

		if( timerVector.size() > 0 )
		{
			nextTimer = timerVector[0]->GetNextRunTS();

			for( t = 1; t < timerVector.size(); t++ )
			{
				nextRun = timerVector[t]->GetNextRunTS();

				if( nextRun < nextTimer )
					nextTimer = nextRun;
			}

			currentTS = time( NULL );
			tv.tv_sec = ( nextTimer - currentTS );
			tv.tv_usec = 0;
		}
		else
		{
			tv.tv_sec = 0;
			tv.tv_usec = 0;
		}

		do
		{
			errno = 0;
			fdStatus = select( sockFD+1, &readSet, &writeSet, 0, &tv );
		}
		while( ( fdStatus < 0 ) && ( errno == EINTR ) );
		
		if( FD_ISSET( sockFD, &readSet ) )
		{
			string strBuffer;

			sock->Recv( buffer, 1024 );
			strBuffer = buffer;
			ParseBuffer( strBuffer );
			memset( buffer, 0, 1024 );
		}
		
		if( fdStatus == 0 || fdStatus == EINVAL )
		{
			for( t = 0; t < timerVector.size(); t++ )
			{
				currentTS = time( NULL );
				timer = timerVector[t];

				if( timer->GetNextRunTS() <= currentTS )
					timer->Run();
			}
		}
	}

	sock->Close();
	serverMap.clear();
	clientMap.clear();
	channelMap.clear();
	accessMap.clear();
	glineMap.clear();
	jupeMap.clear();
}


int Network::ParseBuffer( string strBuffer )
{
	string bufferLine;
	Token tokenizedMsg;

	bufferLine = strBuffer;
	tokenizedMsg.Tokenize( bufferLine );
	ParseServMsg( tokenizedMsg );

	return 0;
}


int Network::ParseServMsg( Token params )
{
	Numeric nSrc, nDst;
	msgStatusType execStatus;
	bool foundHandler = false;
	string srvMsg;

	if( params.numTokens() < 2 )
		return 1;
	
	nSrc = params[0];
	srvMsg = params[1];

	if( params.numTokens() > 2 )
		nDst = params[2];
	else
		nDst = Net->GetConfig( "SERVNUM" );
	
	if( params[0] == "SERVER" || params[0] == "PASS" || params[0] == "ERROR" )
		srvMsg = params[0];
	
	for( int c = 0; ServMsgTab[c].Exec != NULL; c++ )
	{
		if( strCmp( srvMsg, ServMsgTab[c].ServMsg ) )
		{
			foundHandler = true;
			execStatus = ServMsgTab[c].Exec( nSrc, nDst, params );
			break;
		}
	}

	if( !foundHandler )
	{
		Debug( CHAN_DEBUG, "UNKNOWN MESSAGE", "%s", srvMsg.c_str() );
		return 1;
	}

	return 0;
}


int Network::Send( char *format, ... )
{
	va_list arg;
	char *buf = new char[4096];

	va_start( arg, format );
	vsprintf( buf, format, arg );
	va_end( arg );

	return sock->Send( buf );
}


int Network::Connect()
{
	int remotePort = 0;
	string remoteHost;

	sock = new Socket;

	remotePort = atoi( GetConfig( "RemotePort" ) );
	remoteHost = GetConfig( "RemoteHost" );
	return sock->Connect( remoteHost, remotePort );
}


string Network::GetUptime()
{
	time_t secs;
	string strUptime;
	int weeks = 0, days = 0, hours = 0, mins = 0;
	char sWeeks[256], sDays[256], sHours[256], sMins[256], sBuffer[512];

	secs = time( NULL ) - starttime;

	while( secs >= 604800 )
	{
		weeks++;
		secs -= 604800;
	}
	while( secs >= 86400 )
	{
		days++;
		secs -= 86400;
	}
	while( secs >= 3600 )
	{
		hours++;
		secs -= 3600;
	}
	while( secs >= 60 )
	{
		mins++;
		secs -= 60;
	}


	sBuffer[0] = '\0';
	sprintf( sWeeks, " " );
	sprintf( sDays, " " );
	sprintf( sHours, " " );
	sprintf( sMins, " " );

	if( weeks > 0 )
	{
		sprintf( sWeeks, "%d weeks, ", weeks );
		sprintf( sBuffer, "%s%s", sBuffer, sWeeks );
	}
	if( days > 0 )
	{
		sprintf( sDays, "%d days, ", days );
		sprintf( sBuffer, "%s%s", sBuffer, sDays );
	}
	if( hours > 0 )
	{
		sprintf( sHours, "%d hours, ", hours );
		sprintf( sBuffer, "%s%s", sBuffer, sHours );
	}
	if( mins > 0 )
	{
		sprintf( sMins, "%d mins, ", mins );
		sprintf( sBuffer, "%s%s", sBuffer, sMins );
	}

	sprintf( sBuffer, "%s%u seconds", sBuffer, secs );
	strUptime = sBuffer;

	return strUptime;
}


string Network::GetSocketStats()
{
	int s = 0, r = 0;
	float sBytes, rBytes;
	string sUnits, rUnits;
	stringstream sockStats;
	char strRBytes[10], strSBytes[10];

	string units[5] =
	{
		"bytes",
		"KB",
		"MB",
		"GB",
		"TB"
	};

	sBytes = sock->GetBytesSent();
	rBytes = sock->GetBytesRecv();

	for( s = 0; sBytes >= 1024; s++ )
		sBytes /= 1024;
	
	for( r = 0; rBytes >= 1024; r++ )
		rBytes /= 1024;
	
	sprintf( strRBytes, "%0.2f", rBytes );
	sprintf( strSBytes, "%0.2f", sBytes );

	sUnits = units[ s ];
	rUnits = units[ r ];
	
	sockStats << strSBytes << " " << sUnits << " sent, "
	          << strRBytes << " " << rUnits << " received";

	return sockStats.str();
}


Access *Network::AddAccessEntry( string usrNick, int usrLevel, bool usrProtect, bool usrSuspend,
                                 time_t usrLastSeen, string usrPasswd, string usrMasks )
{
	accessMap[ usrNick ] = new Access( usrNick, usrLevel, usrProtect, usrSuspend, 
		usrLastSeen, usrPasswd, usrMasks );
	
	return accessMap[ usrNick ];
}


void Network::RemAccessEntry( string usrNick )
{
	accessMap.erase( usrNick );
}


Access *Network::FindAccessEntry( string usrNick )
{
	AccessMapType::iterator iter = accessMap.begin();

	for( iter = accessMap.begin();
		iter != accessMap.end();
		iter++ )
	{
		if( strCmp( iter->second->GetNick(), usrNick ) )
			return iter->second;
	}

	return NULL;
}


string Network::GetNextNumeric( string ownerNum )
{
	Server *serv = Net->FindServerByNum( ownerNum );
	string nextNum;
	int numCount;

	assert( serv != NULL );

	numCount = serv->NumericCount();
	nextNum = IntToBase64( numCount, 5 );
	nextNum = ownerNum + nextNum.substr( 2, 3 );

	return nextNum;
}


Gline *Network::AddGline( string mask, time_t duration, string reason, string oper )
{
	mask = strLower( mask );
	glineMap[mask] = new Gline( mask, duration, reason, oper );

	assert( glineMap[mask] != NULL );

	return glineMap[mask];
}


Gline *Network::FindGlineByMask( string mask )
{
	GlineMapType::iterator iter;

	mask = strLower( mask );

	for( iter = glineMap.begin(); iter != glineMap.end(); iter++ )
	{
		if( strCmp( iter->second->GetHost(), mask ) )
		{
			return iter->second;
		}
	}

	return NULL;
}


void Network::RemGline( string mask )
{
	GlineMapType::iterator iter;

	for( iter = glineMap.begin(); iter != glineMap.end(); iter++ )
	{
		if( strCmp( iter->second->GetHost(), mask ) )
		{
			glineMap.erase( iter );
		}
	}
}


void Network::SendGlines()
{
	GlineMapType::iterator iter;

	for( iter = glineMap.begin(); iter != glineMap.end(); iter++ )
	{
		iter->second->Register();
	}
}


Jupe *Network::AddJupe( string srvName, time_t srvDuration, string srvDesc, bool active )
{
	srvName = strLower( srvName );
	jupeMap[ srvName ] = new Jupe( srvName, srvDuration, srvDesc, active );

	return jupeMap[ srvName ];
}


Jupe *Network::AddJupe( string srvName, time_t srvDuration, string srvDesc, string srvAdmin, bool active )
{
	srvName = strLower( srvName );
	jupeMap[ srvName ] = new Jupe( srvName, srvDuration, srvDesc, srvAdmin, active );

	return jupeMap[ srvName ];
}


Jupe *Network::AddJupe( string srvName, time_t srvDuration, time_t srvStart, string srvDesc, bool active )
{
	srvName = strLower( srvName );
	jupeMap[ srvName ] = new Jupe( srvName, srvDuration, srvStart, srvDesc, active );

	return jupeMap[ srvName ];
}


Jupe *Network::AddJupe( string srvName, time_t srvDuration, time_t srvStart, string srvDesc, string srvAdmin, bool active )
{
	srvName = strLower( srvName );
	jupeMap[ srvName ] = new Jupe( srvName, srvDuration, srvStart, srvDesc, srvAdmin, active );

	return jupeMap[ srvName ];
}


Jupe *Network::FindJupeByName( string srvName )
{
	srvName = strLower( srvName );
	return jupeMap[ srvName ];
}


void Network::RemJupe( string srvName )
{
	jupeMap.erase( srvName );
}


void Network::RegisterJupes()
{
	JupeMapType::iterator jupe;

	for( jupe = jupeMap.begin(); jupe != jupeMap.end(); jupe++ )
	{
		jupe->second->Register();
	}
}


bool Network::FinishedBurst()
{
	ServerMapType::iterator iter;
	Server *serv;
	
	for( iter = Net->ServIter(); iter != Net->ServTail(); iter++ )
	{
		serv = iter->second;

		if( serv->IsUplink() && serv->CheckFlag( serverFlags::SERV_BURST_ACK ) )
			return true;
	}

	return false;
}

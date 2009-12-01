#include <algorithm>
#include <string>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include "stealth.h"
#include "server.h"
#include "client.h"
#include "channel.h"
#include "token.h"
#include "msgmap.h"
#include "config.h"
#include "types.h"
#include "events.h"

using namespace std;

Network *Net;
vector< Timer* > timerVector;
string runningConfig;
bool connecting;
bool dying;


int strSCmp( string s1, string s2 )
{
	return !strcmp( s1.c_str(), s2.c_str() );
}


int strSCmp( string s1, char *s2 )
{
	return !strcmp( s1.c_str(), s2 );
}


int strSCmp( char *s1, string s2 )
{
	return !strcmp( s1, s2.c_str() );
}


int strSCmp( char *s1, char *s2 )
{
	return !strcmp( s1, s2 );
}


int strCmp( string s1, string s2 )
{
	return !strcasecmp( s1.c_str(), s2.c_str() );
}


int strCmp( string s1, char *s2 )
{
	return !strcasecmp( s1.c_str(), s2 );
}


int strCmp( char *s1, string s2 )
{
	return !strcasecmp( s1, s2.c_str() );
}


int strCmp( char *s1, char *s2 )
{
	return !strcasecmp( s1, s2 );
}


void strCopy( string strBuffer, char *cBuffer ) {
	for( int c = 0; cBuffer[c] != '\0'; c++ ) {
		strBuffer[c] = cBuffer[c];
	}
}


string strUpper( string strBuffer )
{
	string::size_type i = 0, end = strBuffer.size();

	for( i = 0; i < end; i++ )
		strBuffer[ i ] = toupper( strBuffer[ i ] );

	return strBuffer;
}


string strLower( string strBuffer )
{
	string::size_type i = 0, end = strBuffer.size();

	for( i = 0; i < end; i++ )
		strBuffer[ i ] = tolower( strBuffer[ i ] );

	return strBuffer;
}


string intToStr( int i )
{
	stringstream s;
	s << i;
	return s.str();
}


void Debug( DebugMsgType type, char *title, char *format, ... )
{
	va_list arg;
	char *buf = new char[1024];

	va_start( arg, format );
	vsprintf( buf, format, arg );
	va_end( arg );

	switch( type )
	{
		case CHAN_DEBUG:
		case CHAN_INFO:
			AppendLog( "debug.log", "%s: %s", title, buf );

			if( Net->FinishedBurst() )
				Net->Send( "%sAAA P %s :\002%9s:\002 %s\n", Net->GetConfig( "SERVNUM" ),
					Net->GetConfig( "INFOCHAN" ), title, buf );
			break;
		

		case PROG_INFO:
			AppendLog( "debug.log", "%s: %s", title, buf );
			cout << "[DEBUG] " << title << ": " << buf << endl;
			break;
			

		case PROG_ERR:
			AppendLog( "error.log", "%s: %s", title, buf );
			cerr << "[ERROR] " << title << ": " << buf << endl;
			break;
	}
}


void Report( ServerEventType event, string num, int n, ... )
{
	va_list arg;
	char *buf = new char[1024], *format;
	string myNum, infoChan, src;
	int i = 0;
	Server *serv;
	Client *user;
	EventSource sourceType;
	
	for( i = 0; serverEvents[i].event != SRV_END_OF_MAP; i++ )
	{
		if( serverEvents[i].event == event )
		{
			format = serverEvents[i].format;
			sourceType = serverEvents[i].source;
			break;
		}
	}

	if( serverEvents[i].report == false || !Net->FinishedBurst() )
		return;

	va_start( arg, n );
	vsprintf( buf, format, arg );
	va_end( arg );

	myNum = Net->GetConfig( "SERVNUM" );
	myNum += "AAA";
	infoChan = Net->GetConfig( "INFOCHAN" );

	if( num.length() == 2 )
		sourceType = EVT_SERVER;
	else if( num.length() == 5 )
		sourceType = EVT_CLIENT;

	switch( sourceType )
	{
		case EVT_SERVER:
			serv = Net->FindServerByNum( num );
			if( serv != NULL )
				src = serv->GetName();
			else
				src = num;

			if( src.length() > NICKLEN )
				src = src.substr( 0, NICKLEN );
	
			Net->Send( "%s P %s :[%12s*] \002%s\002 %s\n", myNum.c_str(), 
				infoChan.c_str(), src.c_str(), serverEvents[i].token, buf );
			break;

		case EVT_CLIENT:
			user = Net->FindClientByNum( num );
			if( user != NULL )
				src = user->GetNick();
			else
				src = num;

			if( src.length() > NICKLEN )
				src = src.substr( 0, NICKLEN );
	
			Net->Send( "%s P %s :[ %12s] \002%s\002 %s\n", myNum.c_str(), 
				infoChan.c_str(), src.c_str(), serverEvents[i].token, buf );
			break;
	}
}


void Report( CommandEventType event, string num, int n, ... )
{
	va_list arg;
	char *buf = new char[1024], *format = new char[128];
	string myNum, infoChan, src;
	int i = 0;
	Client *user;

	for( i = 0; commandEvents[i].event != CMD_END_OF_MAP; i++ )
	{
		if( commandEvents[i].event == event )
		{
			format = commandEvents[i].format;
			break;
		}
	}

	if( commandEvents[i].report == false || !Net->FinishedBurst() )
		return;
	
	va_start( arg, n );
	vsprintf( buf, format, arg );
	va_end( arg );

	myNum = Net->GetConfig( "SERVNUM" );
	myNum += "AAA";
	infoChan = Net->GetConfig( "INFOCHAN" );
	user = Net->FindClientByNum( num );
	if( user != NULL )
		src = user->GetNick();
	else
		src = num;

	if( src.length() > NICKLEN )
		src = src.substr( 0, NICKLEN );
	
	Net->Send( "%s P %s :[ %12s] %s %s\n", myNum.c_str(), infoChan.c_str(),
		src.c_str(), commandEvents[i].token, buf );
}


bool AppendLog( string logFile, char *format, ... )
{
	va_list arg;
	time_t ts;
	ofstream log( logFile.c_str(), ios::app );
	string timeStamp;
	char *buf = new char[1024];
	int i = 0;

	if( log.fail() )
		return false;

	va_start( arg, format );
	vsprintf( buf, format, arg );
	va_end( arg );

	i = strlen( buf ) - 1;
	while( buf[i] == '\n' || buf[i] == '\r' )
	{
		buf[i] = '\0';
		i--;
	}

	ts = time( NULL );
	timeStamp = ctime( &ts );
	timeStamp = timeStamp.substr( 0, timeStamp.length() - 1 );

	log << timeStamp << " " << buf << endl;

	return true;
}


bool AppendLog( string logFile, const char *format, ... )
{
	va_list arg;
	time_t ts;
	ofstream log( logFile.c_str(), ios::app );
	string timeStamp;
	char *buf = new char[1024];
	int i = 0;

	if( log.fail() )
		return false;

	va_start( arg, format );
	vsprintf( buf, format, arg );
	va_end( arg );

	i = strlen( buf ) - 1;
	while( buf[i] == '\n' || buf[i] == '\r' )
	{
		buf[i] = '\0';
		i--;
	}

	ts = time( NULL );
	timeStamp = ctime( &ts );
	timeStamp = timeStamp.substr( 0, timeStamp.length() - 1 );

	log << timeStamp << " " << buf << endl;

	return true;
}


const char IntToNum[] = {
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
	'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
	'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
	'w','x','y','z','0','1','2','3','4','5','6','7','8','9','[',']'
};

const unsigned int NumToInt[] = {
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	52,53,54,55,56,57,58,59,60,61, 0, 0, 0, 0, 0, 0,
	 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
	15,16,17,18,19,20,21,22,23,24,25,62, 0,63, 0, 0,
	 0,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
	41,42,43,44,45,46,47,48,49,50,51, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


string getServNum( string num )
{
	return num.substr( 0, 2 );
}


string Base64ToIP( string ipBase )
{
	int ipInt = Base64ToInt( ipBase );
	unsigned char *s = ( unsigned char * ) &ipInt;
	unsigned char a, b, c, d;
	static char buf[16];
	string ipAddr;

	a = *s++;
	b = *s++;
	c = *s++;
	d = *s++;

	sprintf( buf, "%u.%u.%u.%u", d, c, b, a );
	ipAddr = buf;

	return ipAddr;
}


int Base64ToInt( string Base64 )
{
	const char *s = Base64.c_str();
	unsigned int i = NumToInt[( unsigned char )*s++];

	while( *s )
	{
		i <<= 6;
		i += NumToInt[( unsigned char )*s++];
	}
	return i;
}


string IntToBase64( int s, int count )
{
	string buf = "     ";

	buf[count] = '\0';

	while( count > 0 )
	{
		buf[--count] = IntToNum[(s & 63)];
		s >>= 6;
	}
	
	return buf;
}


void RegisterTimers()
{
	timerVector.push_back( new Timer( ExpireGlines, 5 ) );
	timerVector.push_back( new Timer( ExpireJupes, 5 ) );
//	timerVector.push_back( new Timer( SaveData, 600 ) );
	timerVector.push_back( new Timer( SaveMap, 60 ) );
	timerVector.push_back( new Timer( SaveStats, 60 ) );
	timerVector.push_back( new Timer( SaveServers, 60 ) );
}


void ExpireGlines()
{
	GlineMapType::iterator gIter;
	time_t currentTS = time( NULL );

	for( gIter = Net->GlineIter(); gIter != Net->GlineTail(); gIter++ )
	{
		if( gIter->second->GetExpireTS() <= currentTS )
			Net->RemGline( gIter->second->GetHost() );
	}
}


void ExpireJupes()
{
	JupeMapType::iterator jIter;
	time_t currentTS = time( NULL );

	for( jIter = Net->JupeIter(); jIter != Net->JupeTail(); jIter++ )
	{
		if( jIter->second == NULL )
			continue;

		if( jIter->second->GetExpireTS() <= currentTS )
			Net->RemJupe( jIter->second->GetName() );
	}
}


void SaveData()
{
	Net->SaveData();
}


void writePid( char *binName )
{
	ofstream pidFile;
	string fileName = binName;

	fileName += ".pid";

	pidFile.open( fileName.c_str() );

	if( pidFile.fail() )
	{
		Debug( PROG_ERR, "writePid", "Could not open pid file \"%s\"", fileName.c_str() );
		return;
	}
	else
	{
		pidFile << getpid() << endl;
		pidFile.close();
	}
}


void SaveMap()
{
	vector<Link> netMap;
	ofstream mapFile;

	GetLinks( netMap, Net->GetConfig( "SERVNUM" ), 0 );
	FormatLinks( netMap );

	mapFile.open( "/www/virtuanet/www/st-dta/map" );

	if( mapFile.fail() )
		return;

	for( unsigned int i = 0; i < netMap.size(); i++ )
	{
		mapFile << netMap[i].name << endl;
	}

	mapFile.close();
}


void SaveStats()
{
	ClientMapType::iterator usr;
	ServerMapType::iterator srv;
	ChannelMapType::iterator chn;
	ofstream statFile;
	int tClients = 0, tServers = 0, tChannels = 0, tOpers = 0;

	for( usr = Net->ClientIter(); usr != Net->ClientTail(); usr++ )
	{
		if( !usr->second->IsService() )
			tClients++;

		if( usr->second->IsOper() )
			tOpers++;
	}

	for( chn = Net->ChannelIter(); chn != Net->ChannelTail(); chn++ )
		tChannels++;

	for( srv = Net->ServIter(); srv != Net->ServTail(); srv++ )
		tServers++;

	statFile.open( "/www/virtuanet/www/st-dta/stats" );
	if( statFile.fail() )
		return;

	statFile << "C" << tClients << endl
		<< "O" << tOpers << endl
		<< "S" << tServers << endl
		<< "H" << tChannels << endl;

	statFile.close();
}


void SaveServers()
{
	ServerMapType::iterator srv;
	ofstream servFile;

	servFile.open( "/www/virtuanet/www/st-dta/servers" );
	if( servFile.fail() )
		return;

	for( srv = Net->ServIter(); srv != Net->ServTail(); srv++ )
	{
		servFile << srv->second->GetName() << " " << srv->second->GetStartTime() << endl;
	}

	servFile.close();
}

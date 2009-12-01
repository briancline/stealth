#include <string>
#include <time.h>
#include "stealth.h"
#include "network.h"
#include "gline.h"

using namespace std;


Gline::Gline( string host, time_t duration )
	: g_host( host ), ts_expire( time( NULL ) + duration ), ts_set( time( NULL ) ),
	  g_numhosts( 0 )
{ 
	FindNumHosts();
}


Gline::Gline( string host, time_t duration, string reason, string oper )
	: g_host( host ), g_reason( reason ), g_oper( oper ), ts_expire( time( NULL ) + duration ), 
	  ts_set( time( NULL ) ), g_numhosts( 0 )
{ 
	FindNumHosts();
}


Gline::Gline( string host, time_t duration, string oper )
	: g_host( host ), g_reason( "No reason specified" ), g_oper( oper ), 
	  ts_expire( time( NULL ) + duration ), ts_set( time( NULL ) ), g_numhosts ( 0 )
{
	FindNumHosts();
}


Gline::Gline( string host, string oper )
	: g_host( host ), g_reason( "No reason specified" ), g_oper( oper ), 
	  ts_expire( time( NULL ) + 3600 ), ts_set( time( NULL ) ), g_numhosts ( 0 )
{
	FindNumHosts();
}


Gline::~Gline()
{
	g_host.erase();
	g_reason.erase();
	g_oper.erase();
}


void Gline::Register()
{
	time_t remaining;
	string timeZone, expire;
	tm *timePtr;
//	int isDST = 0, gmtOffset = 0;

	remaining = ts_expire - time( NULL );

	timePtr = localtime( &ts_expire );
	timeZone = timePtr->tm_zone;

//	if( timePtr->tm_isdst )
//		isDST = 1;
//	gmtOffset = timePtr->tm_gmtoff / 3600 - isDST;

	expire = ctime( &ts_expire );
	expire = expire.substr( 0, expire.length() - 1 );
	
	Net->Send( "%s GL * +%s %ld :Banned %s[%d] until %s %s [%ld]: %s\n", 
		Net->GetConfig( "SERVNUM" ), g_host.c_str(), remaining, g_host.c_str(), 
		g_numhosts, expire.c_str(), timeZone.c_str(), ts_expire, g_reason.c_str() );
}


string Gline::GetExpireTime()
{
	string fullTime = ctime( &ts_expire );
	fullTime = fullTime.substr( 0, fullTime.length() - 1 );

	return fullTime;
}


string Gline::GetSetTime()
{
	string fullTime = ctime( &ts_set );
	fullTime = fullTime.substr( 0, fullTime.length() - 1 );

	return fullTime;
}


int Gline::FindNumHosts()
{
	ClientMapType::iterator iter;
	Client *user;
	string hostMask, ipMask;

	g_numhosts = 0;

	for( iter = Net->ClientIter(); iter != Net->ClientTail(); iter++ )
	{
		user = iter->second;
		hostMask = user->GetUserName() + "@" + user->GetHostName();
		ipMask = user->GetUserName() + "@" + user->GetIP();

		if( match( g_host, hostMask ) || match( g_host, ipMask ) )
			g_numhosts++;
	}

	return g_numhosts;
}

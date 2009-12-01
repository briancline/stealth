#include <list>
#include <string>
#include "access.h"
#include "token.h"
#include "stealth.h"


Access::Access()
	: a_level( 0 ), a_protect( false ), a_suspend( false )
{ }


Access::~Access()
{
	a_hosts.erase( a_hosts.begin(), a_hosts.end() );
}


Access::Access( string usrNick, int usrLevel, bool usrProtect, bool usrSuspend, 
                time_t usrLastSeen, string usrPasswd, string usrMasks )
	: a_nick( usrNick ), a_pass( usrPasswd ), a_level( usrLevel ), 
	  a_protect( usrProtect ), a_suspend( usrSuspend ), 
	  a_lastSeen( usrLastSeen )
{
	Token hostList( usrMasks );
	int i = 0;

	assert( hostList.numTokens() > 0 );

	for( i = 0; i < hostList.numTokens(); i++ )
	{
		AddHost( hostList[i] );
	}
}


string Access::GetNick()
{
	return a_nick;
}


string Access::GetPass()
{
	return a_pass;
}


int Access::GetLevel()
{
	return a_level;
}


string Access::GetMasks()
{
	list<string>::iterator iter = a_hosts.begin();
	string strMasks;

	for( iter = a_hosts.begin(); iter != a_hosts.end(); iter++ )
	{
		if( iter != a_hosts.begin() )
			strMasks.append( " " );
		
		strMasks.append( *iter );
	}

	return strMasks;
}


string Access::GetLastSeen()
{
	string fullTime = ctime( &a_lastSeen );
	fullTime = fullTime.substr( 0, fullTime.length() - 1 );

	return fullTime;
}


time_t Access::GetLastTS()
{
	return a_lastSeen;
}


bool Access::IsProtected()
{
	return a_protect;
}


bool Access::IsSuspended()
{
	return a_suspend;
}


bool Access::IsValidHost( string fullHost )
{
	Token hostList( this->GetHostList() );
	int i = 0;

	for( i = 0; i < hostList.numTokens(); i++ )
	{
		if( match( hostList[i], fullHost ) )
			return true;
	}

	return false;
}


void Access::SetNick( string nick )
{
	a_nick = nick;
}


void Access::SetLevel( int level )
{
	a_level = level;
}


void Access::SetProtect( bool protect )
{
	a_protect = protect;
}


void Access::SetSuspend( bool suspend )
{
	a_suspend = suspend;
}


void Access::SetTS( time_t lastSeen )
{
	a_lastSeen = lastSeen;
}


void Access::SetPassword( string pass )
{
	a_pass = pass;
}


int Access::AddHost( string hostmask )
{
	list<string>::iterator h;

	for( h = a_hosts.begin(); h != a_hosts.end(); h++ ) 
	{
		if( strCmp( *h, hostmask ) )
			return 1;
	}
	a_hosts.push_back( hostmask );
	return 0;
}


int Access::DelHost( string hostmask )
{
	list<string>::iterator h;
	
	for( h = a_hosts.begin(); h != a_hosts.end(); h++ ) 
	{
		if( *h == hostmask )
		{
			a_hosts.erase( h );
			return 0;
		}
	}
	return 1;
}


string Access::GetHostList()
{
	list<string>::iterator i;
	string strHosts;

	for( i = a_hosts.begin(); i != a_hosts.end(); i++ )
	{
		if( i != a_hosts.begin() )
			strHosts.append( " " );

		strHosts.append( (*i) );
	}

	return strHosts;
}

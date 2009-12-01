#include "server.h"
#include "stealth.h"

using namespace std;


Server::Server( string uplinkNum, string serverName, int hops, time_t starttime, string serverNum, 
                int maxconn, string desc, bool uplink )
	: s_flags( serverFlags::SERV_NONE ), s_name( serverName ), s_desc( desc ),
	  s_maxConnections( maxconn ), s_hopcount( hops ), s_starttime( starttime ), s_num( serverNum ),
	  s_ownernum( uplinkNum ), s_numCount( 0 )
	  
{
	s_intNum = Base64ToInt( serverNum );
	if( uplink ) {
		AddFlag( serverFlags::SERV_UPLINK );
	}
}


void Server::AddFlag( flagType whichFlag )
{
	if( !CheckFlag( whichFlag ) )
	{
		s_flags |= whichFlag;
	}
}


void Server::RemFlag( flagType whichFlag )
{
	if( CheckFlag( whichFlag ) )
	{
		s_flags &= ~whichFlag;
	}
}


bool Server::CheckFlag( flagType whichFlag )
{
	return ( ( s_flags & whichFlag ) == whichFlag );
}


bool Server::IsUplink()
{
	return this->CheckFlag( serverFlags::SERV_UPLINK );
}


void Server::NumericSet( string num )
{
	s_numCount = Base64ToInt( num.substr( 2, 3 ) );
}


void Server::NumericInc()
{
	s_numCount++;
}


int Server::NumericCount()
{
	return s_numCount;
}

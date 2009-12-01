#include "jupe.h"
#include "stealth.h"

using namespace std;


Jupe::Jupe( string jupeName, time_t jupeDuration, string jupeDesc, bool active )
	: s_name( jupeName ), s_desc( jupeDesc ), s_starttime( time( NULL ) ),
	  s_duration( jupeDuration ), s_expire( s_starttime + s_duration ),
	  s_active( active )
{

}


Jupe::Jupe( string jupeName, time_t jupeDuration, string jupeDesc, string jupeAdmin, bool active )
	: s_name( jupeName ), s_desc( jupeDesc ), s_admin( jupeAdmin ),
	  s_starttime( time( NULL ) ), s_duration( jupeDuration ), 
	  s_expire( s_starttime + s_duration ), s_active( active )
{

}


Jupe::Jupe( string jupeName, time_t jupeDuration, time_t jupeStart, string jupeDesc, bool active )
	: s_name( jupeName ), s_desc( jupeDesc ), s_starttime( jupeStart ),
	  s_duration( jupeDuration ), s_expire( time( NULL ) + s_duration ),
	  s_active( active )
{

}


Jupe::Jupe( string jupeName, time_t jupeDuration, time_t jupeStart, string jupeDesc, string jupeAdmin, bool active )
	: s_name( jupeName ), s_desc( jupeDesc ), s_admin( jupeAdmin ), 
	  s_starttime( jupeStart ), s_duration( jupeDuration ),
	  s_expire( time( NULL ) + s_duration ), s_active( active )
{

}


string Jupe::GetExpireString()
{
	time_t expireTS = this->GetExpireTS();
	string expireTime = ctime( &expireTS );

	expireTime = expireTime.substr( 0, expireTime.length() - 1 );

	return expireTime;
}


void Jupe::Register()
{
	time_t remaining = this->GetRemainingTime();
	char actChar;

	if( s_active )
		actChar = '+';
	else
		actChar = '-';
	
	Net->Send( "%s JU * %c%s %ld %ld :%s\n",
		Net->GetConfig( "SERVNUM" ),
		actChar,
		s_name.c_str(),
		remaining,
		s_starttime,
		s_desc.c_str() );
}


void Jupe::Activate()
{
	s_active = true;
	this->Register();
}


void Jupe::Deactivate()
{
	s_active = false;
	this->Register();
}


time_t Jupe::GetRemainingTime()
{
	time_t remaining;

	remaining = this->GetExpireTS() - time( NULL );

	return remaining;
}

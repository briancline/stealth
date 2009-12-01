#include <string>
#include "client.h"
#include "stealth.h"
#include "network.h"
#include "chanclient.h"
#include "flags.h"

using namespace std;
using namespace chanClientFlags;


ChannelClient::ChannelClient()
	: c_modes( CHAN_NONE )
{ }


ChannelClient::ChannelClient( Client *user )
	: c_user( user ), c_modes( CHAN_NONE )
{ }


ChannelClient::ChannelClient( Client *user, modeType &modes )
	: c_user( user ), c_modes( modes )
{ }


ChannelClient::ChannelClient( Client *user, string chanName, modeType &modes )
	: c_user( user ), c_modes( modes ), c_cname( chanName )
{ }


bool ChannelClient::CheckMode( modeType mode )
{
	return ( mode == ( c_modes & mode ) );
}


bool ChannelClient::IsOp()
{
	return CheckMode( CHAN_OP );
}


bool ChannelClient::IsVoice()
{
	return CheckMode( CHAN_VOICE );
}


void ChannelClient::SetMode( modeType mode )
{
	c_modes |= mode;
}


void ChannelClient::SetOp()
{
	SetMode( CHAN_OP );
}


void ChannelClient::SetVoice()
{
	SetMode( CHAN_VOICE );
}


void ChannelClient::RemMode( modeType mode )
{
	c_modes &= ~mode;
}


void ChannelClient::RemOp()
{
	RemMode( CHAN_OP );
}


void ChannelClient::RemVoice()
{
	RemMode( CHAN_VOICE );
}


Client *ChannelClient::GetClient()
{
	return c_user;
}


string ChannelClient::GetNick()
{
	return c_user->GetNick();
}


string ChannelClient::GetUserName()
{
	return c_user->GetUserName();
}


string ChannelClient::GetHostName()
{
	return c_user->GetHostName();
}


string ChannelClient::GetNumeric()
{
	return c_user->GetNumeric();
}


string ChannelClient::GetIP()
{
	return c_user->GetIP();
}


string ChannelClient::GetChanName()
{
	return c_cname;
}


void ChannelClient::SetChanName( string chanName )
{
	c_cname = chanName;
}

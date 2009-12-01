#include "stealth.h"
#include "channel.h"
#include "access.h"

using namespace std;
using namespace clientFlags;


Client::Client( string ownerNum, string nick, int hops, int starttime, string ident, string host,
                string modes, string ip, string num, string realname )
: c_modes( USER_NONE ), c_starttime( starttime ), c_nick( nick ), c_ident( ident ), c_host( host ),
  c_realname( realname ), ipOct( Base64ToIP( ip ) ), ipBase( ip ), c_num( num )
{
	c_access = NULL;
	
	for( unsigned int m = 0; m < modes.length(); m++ )
	{
		switch( modes[m] )
		{
			case '+':	break;
			case 'd':	AddMode( USER_DEAF );		break;
			case 'i':	AddMode( USER_INVISIBLE );	break;
			case 'o':	AddMode( USER_OPER );		break;
			case 'w':	AddMode( USER_WALLOPS );	break;
			case 'k':	AddMode( USER_SERVICE );	break;
			case 'g':	AddMode( USER_HACKS );		break;
			case 'r':	AddMode( USER_ACCOUNT );   break;
			case 'x':	AddMode( USER_HIDEHOST );  break;
		}
	}
}


void Client::AddMode( modeType whichMode )
{
	if( !CheckMode( whichMode ) )
	{
		c_modes |= whichMode;
	}
}


void Client::AddMode( char whichMode )
{
	switch( whichMode )
	{
		case 'd':	AddMode( USER_DEAF );       break;
		case 'i':	AddMode( USER_INVISIBLE );  break;
		case 'o':	AddMode( USER_OPER );       break;
		case 'w':	AddMode( USER_WALLOPS );    break;
		case 'k':	AddMode( USER_SERVICE );    break;
		case 'g':	AddMode( USER_HACKS );      break;
		case 'r':	AddMode( USER_ACCOUNT );    break;
		case 'x':	AddMode( USER_HIDEHOST );   break;
	}
}


void Client::RemMode( modeType whichMode )
{
	if( CheckMode( whichMode ) )
	{
		c_modes &= ~whichMode;
	}
}


void Client::RemMode( char whichMode )
{
	switch( whichMode )
	{
		case 'd':	RemMode( USER_DEAF );       break;
		case 'i':	RemMode( USER_INVISIBLE );  break;
		case 'o':	RemMode( USER_OPER );       break;
		case 'w':	RemMode( USER_WALLOPS );    break;
		case 'k':	RemMode( USER_SERVICE );    break;
		case 'g':	RemMode( USER_HACKS );      break;
		case 'r':	RemMode( USER_ACCOUNT );    break;
		case 'x':	RemMode( USER_HIDEHOST );   break;
	}
}


bool Client::CheckMode( modeType whichMode )
{
	return ( ( c_modes & whichMode ) == whichMode );
}


void Client::SetAccount( string userName )
{
	c_account = userName;

	this->AddMode( USER_ACCOUNT );
	this->SetAccessRecord( userName );
	this->SetHiddenHost();
}


string Client::GetAccount()
{
	return c_account;
}


void Client::SetHiddenHost()
{
	c_hiddenhost = c_account;
	c_hiddenhost.append( "." );
	c_hiddenhost.append( Net->GetConfig( "HIDDENHOST" ) );
}


string Client::GetHiddenHost()
{
	if( this->HasAccount() )
		return c_hiddenhost;
	else
		return c_host;
}


bool Client::IsOper()
{
	return ( ( c_modes & USER_OPER ) == USER_OPER );
}


bool Client::IsService()
{
	return ( ( c_modes & USER_SERVICE ) == USER_SERVICE );
}


bool Client::IsHostHidden()
{
	return ( ( c_modes & USER_HIDEHOST ) == USER_HIDEHOST );
}


bool Client::HasAccount()
{
	return ( ( c_modes & USER_ACCOUNT ) == USER_ACCOUNT );
}


string Client::GetModeString()
{
	string modes = "+";

	if( this->CheckMode( USER_DEAF ) )
		modes.append( "d" );
	if( this->CheckMode( USER_INVISIBLE ) )
		modes.append( "i" );
	if( this->CheckMode( USER_OPER ) )
		modes.append( "o" );
	if( this->CheckMode( USER_WALLOPS ) )
		modes.append( "w" );
	if( this->CheckMode( USER_SERVICE ) )
		modes.append( "k" );
	if( this->CheckMode( USER_HACKS ) )
		modes.append( "g" );
	if( this->CheckMode( USER_ACCOUNT ) )
		modes.append( "r" );
	if( this->CheckMode( USER_HIDEHOST ) )
		modes.append( "x" );
	
	return modes;
}


string Client::GetConnectTime()
{
	string fullTime;

	fullTime = ctime( &c_starttime );
	fullTime = fullTime.substr( 0, fullTime.length() - 1 );

	return fullTime;
}


Access *Client::GetAccessRecord()
{
	return c_access;
}


void Client::SetAccessRecord( string userName )
{
	Access *acc = Net->FindAccessEntry( userName );

	if( acc != NULL )
	{
		c_access = acc;
		c_access->SetTS( time( NULL ) );
	}
	else
	{
		this->ClearAccess();
	}
}


void Client::ClearAccess()
{
	c_access = NULL;
}


bool Client::IsAuthed()
{
	if( c_access == NULL )
		return false;
	else
		return true;
}


bool Client::IsSuspended()
{
	if( !IsAuthed() )
		return false;
	else
		return c_access->IsSuspended();
}


bool Client::IsProtected()
{
	if( !IsAuthed() )
		return false;
	else
		return c_access->IsProtected();
}


int Client::GetLevel()
{
	if( !IsAuthed() )
	{
		if( IsOper() )
			return 0;
		else
			return -1;
	}
	else
	{
		return c_access->GetLevel();
	}
}


string Client::GetChanList()
{
	map<string, ChannelClient*, less<string> >::iterator iter;
	string chanString;

	for( iter = c_channels.begin(); iter != c_channels.end(); iter++ )
	{
		if( iter != c_channels.begin() )
			chanString.append( " " );

		if( iter->second->IsVoice() )
			chanString.append( "+" );
		if( iter->second->IsOp() )
			chanString.append( "@" );

		chanString.append( iter->second->GetChanName() );
	}

	return chanString;
}


void Client::AddChan( string chanName )
{
	Channel *chan = Net->FindChannelByName( chanName );
	ChannelClient *chanUsr = chan->FindUser( c_num );

	chanName = strLower( chanName );
	c_channels[chanName] = chanUsr;
}


void Client::RemChan( string chanName )
{
	chanName = strLower( chanName );
	map< string, ChannelClient*, less<string> >::iterator iter;

	for( iter = c_channels.begin(); iter != c_channels.end(); iter++ )
	{
		if( strCmp( iter->second->GetChanName(), chanName ) )
		{
			c_channels.erase( iter );
			break;
		}
	}
}


bool Client::IsOnChan( string chanName )
{
	chanName = strLower( chanName );

	if( c_channels[chanName] != NULL )
		return true;
	else
		return false;
}


void Client::OpOnAllChans()
{
	map< string, ChannelClient*, less<string> >::iterator iter;
	ChannelClient *chanUser;

	for( iter = c_channels.begin(); iter != c_channels.end(); iter++ )
	{
		chanUser = iter->second;
		chanUser->SetOp();
		Net->Send( "%s M %s +o %s\n", Net->GetConfig( "SERVNUM" ),
			chanUser->GetChanName().c_str(), c_num.c_str() );
	}
}


void Client::RemAllChans()
{
	map< string, ChannelClient*, less<string> >::iterator iter;
	Channel *chan;

	for( iter = c_channels.begin(); iter != c_channels.end(); iter++ )
	{
		chan = Net->FindChannelByName( iter->second->GetChanName() );
		chan->RemUser( c_num );
	}

	c_channels.erase( c_channels.begin(), c_channels.end() );
}

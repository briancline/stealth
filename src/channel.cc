#include <string>
#include <map>
#include <list>
#include <sstream>
#include "stealth.h"
#include "network.h"
#include "channel.h"
#include "chanclient.h"

using namespace std;
using namespace channelFlags;
using namespace chanClientFlags;


Channel::Channel( string chanName, time_t createTime, string modes ) 
	: c_name( chanName ), c_modes( channelFlags::CHAN_NONE ), 
	  c_createTime( createTime ), c_limit( 0 )
{
	SetModes( modes );
}


string Channel::GetCreateTime()
{
	string fullTime;

	fullTime = ctime( &c_createTime );
	fullTime = fullTime.substr( 0, fullTime.length() - 1 );

	return fullTime;
}


string Channel::GetModeString()
{
	string modeStr = "+";

	if( CheckMode( CHAN_INVITE ) )
		modeStr.append( "i" );
	if( CheckMode( CHAN_MODERATED ) )
		modeStr.append( "m" );
	if( CheckMode( CHAN_NOEXMSGS ) )
		modeStr.append( "n" );
	if( CheckMode( CHAN_PRIVATE ) )
		modeStr.append( "p" );
	if( CheckMode( CHAN_SECRET ) )
		modeStr.append( "s" );
	if( CheckMode( CHAN_TOPICOP ) )
		modeStr.append( "t" );
	if( CheckMode( CHAN_LIMIT ) )
		modeStr.append( "l" );
	if( CheckMode( CHAN_KEY ) )
		modeStr.append( "k" );
	
	if( CheckMode( CHAN_LIMIT ) )
	{
		modeStr.append( " " );
		modeStr.append( intToStr( GetLimit() ) );
	}
	if( CheckMode( CHAN_KEY ) )
	{
		modeStr.append( " " );
		modeStr.append( GetKey() );
	}

	return modeStr;
}


string Channel::GetUsersString()
{
	string userStr;
	ChanClientMapType::iterator user;

	for( user = c_users.begin(); user != c_users.end(); user++ )
	{
		if( user != c_users.begin() )
			userStr.append( " " );

		if( user->second->IsVoice() )
			userStr.append( "+" );

		if( user->second->IsOp() )
			userStr.append( "@" );

		userStr.append( user->second->GetNick() );
	}

	return userStr;
}


string Channel::GetNumericsString()
{
	string numStr;
	ChanClientMapType::iterator iter;
	ChannelClient *user;

	for( iter = c_users.begin(); iter != c_users.end(); iter++ )
	{
		user = iter->second;

		if( iter != c_users.begin() )
			numStr.append( "," );
		
		numStr.append( user->GetNumeric() );

		if( user->IsVoice() || user->IsOp() )
		{
			numStr.append( ":" );

			if( user->IsOp() )
				numStr.append( "o" );

			if( user->IsVoice() )
				numStr.append( "v" );
		}
	}

	return numStr;
}


void Channel::OpAll()
{
	ChanClientMapType::iterator user;
	string opString, modeString;
	int opCount = 0, i = 0;

	for( user = c_users.begin(); user != c_users.end(); user++ )
	{
		if( !user->second->IsOp() )
		{
			user->second->SetOp();

			if( opCount > 0 )
				opString.append( " " );

			opString.append( user->second->GetNumeric() );
			opCount++;

			if( opCount == 6 )
			{
				Net->Send( "%s M %s +oooooo %s\n", Net->GetConfig("SERVNUM"), c_name.c_str(), opString.c_str() );
				opString.erase();
				opCount = 0;
			}
		}
	}

	if( opCount > 0 )
	{
		modeString.erase();
		for( i = 0; i < opCount; i++ )
			modeString.append( "o" );

		Net->Send( "%s M %s +%s %s\n", Net->GetConfig("SERVNUM"), c_name.c_str(), 
			modeString.c_str(), opString.c_str() );
	}
}


void Channel::VoiceAll()
{
	ChanClientMapType::iterator user;
	string voiceString, modeString;
	int voiceCount = 0, i = 0;

	for( user = c_users.begin(); user != c_users.end(); user++ )
	{
		if( !user->second->IsVoice() )
		{
			user->second->SetVoice();

			if( voiceCount > 0 )
				voiceString.append( " " );

			voiceString.append( user->second->GetNumeric() );
			voiceCount++;

			if( voiceCount == 6 )
			{
				Net->Send( "%s M %s +vvvvvv %s\n", Net->GetConfig("SERVNUM"), c_name.c_str(), 
					voiceString.c_str() );
				voiceString.erase();
				voiceCount = 0;
			}
		}
	}

	if( voiceCount > 0 )
	{
		modeString.erase();
		for( i = 0; i < voiceCount; i++ )
			modeString.append( "v" );

		Net->Send( "%s M %s +%s %s\n", Net->GetConfig("SERVNUM"), c_name.c_str(), 
			modeString.c_str(), voiceString.c_str() );
	}
}


void Channel::DeopAll()
{
	ChanClientMapType::iterator user;
	string deopString, modeString;
	int deopCount = 0, i = 0;

	for( user = c_users.begin(); user != c_users.end(); user++ )
	{
		if( user->second->IsOp() )
		{
			user->second->RemOp();

			if( deopCount > 0 )
				deopString.append( " " );

			deopString.append( user->second->GetNumeric() );
			deopCount++;

			if( deopCount == 6 )
			{
				Net->Send( "%s M %s -oooooo %s\n", Net->GetConfig("SERVNUM"), c_name.c_str(), 
					deopString.c_str() );
				deopString.erase();
				deopCount = 0;
			}
		}
	}

	if( deopCount > 0 )
	{
		modeString.erase();
		for( i = 0; i < deopCount; i++ )
			modeString.append( "o" );

		Net->Send( "%s M %s -%s %s\n", Net->GetConfig("SERVNUM"), c_name.c_str(), 
			modeString.c_str(), deopString.c_str() );
	}
}


void Channel::DevoiceAll()
{
	ChanClientMapType::iterator user;
	string devoiceString, modeString;
	int devoiceCount = 0, i = 0;

	for( user = c_users.begin(); user != c_users.end(); user++ )
	{
		if( user->second->IsVoice() )
		{
			user->second->RemVoice();

			if( devoiceCount > 0 )
				devoiceString.append( " " );

			devoiceString.append( user->second->GetNumeric() );
			devoiceCount++;

			if( devoiceCount == 6 )
			{
				Net->Send( "%s M %s -vvvvvv %s\n", Net->GetConfig("SERVNUM"), c_name.c_str(), 
					modeString.c_str(), devoiceString.c_str() );
				devoiceString.erase();
				devoiceCount = 0;
			}
		}
	}

	if( devoiceCount > 0 )
	{
		modeString.erase();
		for( i = 0; i < devoiceCount; i++ )
			modeString.append( "v" );

		Net->Send( "%s M %s -%s %s\n", Net->GetConfig("SERVNUM"), c_name.c_str(), 
			modeString.c_str(), devoiceString.c_str() );
	}
}


void Channel::ClearOps()
{
	ChanClientMapType::iterator user;

	for( user = c_users.begin(); user != c_users.end(); user++ )
	{
		if( user->second->IsOp() )
			user->second->RemOp();
	}
}


void Channel::ClearVoices()
{
	ChanClientMapType::iterator user;

	for( user = c_users.begin(); user != c_users.end(); user++ )
	{
		if( user->second->IsVoice() )
			user->second->RemVoice();
	}
}


string Channel::GetBansString()
{
	string banStr;
	list<string>::iterator ban;

	for( ban = c_bans.begin(); ban != c_bans.end(); ban++ )
	{
		if( ban != c_bans.begin() )
			banStr.append( " " );

		banStr.append( (*ban) );
	}

	return banStr;
}


void Channel::SetModes( string modes )
{
	unsigned int i = 0;
	Token modeTokens( modes );

	for( i = 0; i < modes.length() && modes[i] != ' '; i++ ) {
		switch( modes[i] ) {
			case '+':	break;
			case 'i':	AddMode( channelFlags::CHAN_INVITE );     break;
			case 'k':	AddMode( channelFlags::CHAN_KEY );        break;
			case 'l':	AddMode( channelFlags::CHAN_LIMIT );      break;
			case 'm':	AddMode( channelFlags::CHAN_MODERATED );  break;
			case 'n':	AddMode( channelFlags::CHAN_NOEXMSGS );   break;
			case 'p':	AddMode( channelFlags::CHAN_PRIVATE );    break;
			case 's':	AddMode( channelFlags::CHAN_SECRET );     break;
			case 't':	AddMode( channelFlags::CHAN_TOPICOP );    break;
		}
	}

	if( modeTokens.numTokens() > 0 )
	{
		if( CheckMode( CHAN_LIMIT ) && CheckMode( CHAN_KEY ) )
		{
			SetLimit( atoi( modeTokens[1].c_str() ) );
			SetKey( modeTokens[2] );
		}
		else if( CheckMode( CHAN_LIMIT ) )
		{
			SetLimit( atoi( modeTokens[1].c_str() ) );
		}
		else if( CheckMode( CHAN_KEY ) )
		{
			SetKey( modeTokens[1] );
		}
	}
}


void Channel::AddMode( modeType whichMode )
{
	if( !CheckMode( whichMode ) )
	{
		c_modes |= whichMode;
	}
}


void Channel::AddMode( char whichMode )
{
	switch( whichMode )
	{
		case 'i':	AddMode( channelFlags::CHAN_INVITE );    break;
		case 'k':	AddMode( channelFlags::CHAN_KEY );       break;
		case 'l':	AddMode( channelFlags::CHAN_LIMIT );     break;
		case 'm':	AddMode( channelFlags::CHAN_MODERATED ); break;
		case 'n':	AddMode( channelFlags::CHAN_NOEXMSGS );  break;
		case 'p':	AddMode( channelFlags::CHAN_PRIVATE );   break;
		case 's':	AddMode( channelFlags::CHAN_SECRET );    break;
		case 't':	AddMode( channelFlags::CHAN_TOPICOP );   break;
	}
}


void Channel::RemMode( modeType whichMode )
{
	if( CheckMode( whichMode ) )
	{
		c_modes &= ~whichMode;
	}
}


void Channel::RemMode( char whichMode )
{
	switch( whichMode )
	{
		case 'i':	RemMode( channelFlags::CHAN_INVITE );    break;
		case 'k':	RemMode( channelFlags::CHAN_KEY );       break;
		case 'l':	RemMode( channelFlags::CHAN_LIMIT );     break;
		case 'm':	RemMode( channelFlags::CHAN_MODERATED ); break;
		case 'n':	RemMode( channelFlags::CHAN_NOEXMSGS );  break;
		case 'p':	RemMode( channelFlags::CHAN_PRIVATE );   break;
		case 's':	RemMode( channelFlags::CHAN_SECRET );    break;
		case 't':	RemMode( channelFlags::CHAN_TOPICOP );   break;
	}
}


bool Channel::CheckMode( modeType whichMode )
{
	return ( ( c_modes & whichMode ) == whichMode );
}


void Channel::ClearModes()
{
	c_modes = channelFlags::CHAN_NONE;
}


void Channel::AddUser( string userNum, string userFlags )
{
	unsigned int i = 0;
	Client *user = Net->FindClientByNum( userNum );
	flagType flags = chanClientFlags::CHAN_NONE;

	for( i = 0; i < userFlags.length(); i++ )
	{
		switch( userFlags[i] )
		{
			case '+':
				break;

			case 'o':
				flags |= chanClientFlags::CHAN_OP;
				break;
				
			case 'v':
				flags |= chanClientFlags::CHAN_VOICE;
				break;
				
			default:
				Debug( PROG_ERR, "ERROR", 
					"Caught unknown channel mode '%c' while trying to add %s to %s", 
					userFlags[i], userNum.c_str(), c_name.c_str() );
				break;
		}
	}

	if( !this->FindUser( userNum ) )
	{
		c_users[ userNum ] = new ChannelClient( user, c_name, flags );
		assert( c_users[userNum] != NULL );

		user->AddChan( c_name );
	}
}


void Channel::AddUser( string userNum )
{
	Client *user = Net->FindClientByNum( userNum );
	flagType flags = chanClientFlags::CHAN_NONE;
	assert( user != NULL );
	
//	if( !this->FindUser( userNum ) )
//	{
		c_users[ userNum ] = new ChannelClient( user, c_name, flags );
		assert( c_users[userNum] != NULL );

		user->AddChan( c_name );
//	}
}


void Channel::RemUser( string userNum )
{
	Client *user = Net->FindClientByNum( userNum );
	map< string, ChannelClient* >::iterator iter = c_users.begin();

	for( iter = c_users.begin();
			iter != c_users.end();
			iter++ )
	{
		if( strCmp( iter->second->GetNumeric(), userNum ) )
		{
			c_users.erase( iter );
			break;
		}
	}

	if( user != NULL )
		user->RemChan( c_name );
	
	if( !this->HasUsers() )
		Net->RemChannel( c_name );
}


ChannelClient *Channel::FindUser( string userNum )
{
	map< string, ChannelClient* >::iterator iter = c_users.begin();

	for( iter = c_users.begin();
			iter != c_users.end();
			iter++ )
	{
		if( strCmp( iter->first, userNum ) )
		{
			return iter->second;
		}
	}
	return NULL;
}


int Channel::UserCount()
{
	return c_users.size();
}


bool Channel::HasUsers()
{
	return !c_users.empty();
}


void Channel::AddBan( string banMask )
{
	c_bans.push_back( banMask );
}


void Channel::RemBan( string banMask )
{
	list<string>::iterator iter = c_bans.begin();

	for( iter = c_bans.begin(); iter != c_bans.end(); iter++ )
	{
		if( strSCmp( (*iter), banMask ) )
		{
			c_bans.erase( iter );
			return;
		}
	}
}


int Channel::BanCount()
{
	return c_bans.size();
}


bool Channel::HasBans()
{
	return !c_bans.empty();
}


void Channel::ClearBans()
{
	list<string>::iterator iter;
	string banString, modeString;
	int banCount = 0, i = 0;

	for( iter = c_bans.begin();
	     iter != c_bans.end();
	     iter++ )
	{
		if( banCount > 0 )
			banString.append( " " );

		banString.append( (*iter) );
		banCount++;
		
		if( banCount == 6 )
		{
			modeString.erase();
			modeString.append( "b", banCount );

			Net->Send( "%s M %s -bbbbbb %s\n", Net->GetConfig("SERVNUM"), c_name.c_str(), 
				banString.c_str() );

			banString.erase();
			banCount = 0;
		}
	}

	c_bans.clear();

	if( banCount > 0 )
	{
		modeString.erase();
		for( i = 0; i < banCount; i++ )
			modeString.append( "b" );

		Net->Send( "%s M %s -%s %s\n", Net->GetConfig("SERVNUM"), c_name.c_str(), 
			modeString.c_str(), banString.c_str() );
	}
}

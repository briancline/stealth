#ifndef __CHANNEL_H__
#define __CHANNEL_H__ "$Id: channel.h,v 1.19 2002/04/16 01:28:57 bcline Exp $"

#include <map>
#include <string>
#include <list>
#include "stealth.h"
#include "client.h"
#include "chanclient.h"
#include "types.h"
#include "flags.h"


typedef map<string, ChannelClient*, less<string> > ChanClientMapType;

class Channel
{
	public:

		Channel()
			: c_modes( channelFlags::CHAN_NONE ) {}

		Channel( string chanName, time_t created )
			: c_name( chanName ), c_modes( channelFlags::CHAN_NONE ), c_createTime( created ) {}

		Channel( string chanName, time_t createTime, string modes );

		~Channel() {}


		string GetName()
			{ return c_name; }

		time_t GetTS()
			{ return c_createTime; }

		string GetCreateTime();
		string GetModeString();
		string GetUsersString();
		string GetBansString();
		string GetNumericsString();

		modeType GetModes()
			{ return c_modes; }

		string GetTopic()
			{ return c_topic; }

		string GetKey()
			{ return c_key; }

		long GetLimit()
			{ return c_limit; }


		void SetName( string chanName )
			{ c_name = chanName; }
			
		void SetTS( time_t timestamp )
			{ c_createTime = timestamp; }

		void SetTopic( string topic )
			{ c_topic = topic; }

		void SetKey( string key )
			{ c_key = key; }

		void SetLimit( int limit )
			{ c_limit = limit; }

		void SetModes( string modes );


		void AddMode( modeType whichMode );
		void AddMode( char whichMode );
		void RemMode( modeType whichMode );
		void RemMode( char whichMode );
		bool CheckMode( modeType whichMode );
		void ClearModes();
		
		void AddUser( string userNum, string userFlags );
		void AddUser( string userNum );
		void RemUser( string userNum );
		ChannelClient *FindUser( string userNum );
		int UserCount();
		bool HasUsers();

		void VoiceAll();
		void OpAll();
		void DevoiceAll();
		void DeopAll();
		void ClearOps();
		void ClearVoices();

		void AddBan( string banMask );
		void RemBan( string banMask );
		int BanCount();
		bool HasBans();
		void ClearBans();
			
		
	private:

		string c_name;
		
		modeType c_modes;
		time_t c_createTime;
		ChanClientMapType c_users;
		list<string> c_bans;
		
		string c_topic;
		string c_key;
		long c_limit;
};


#endif // __CHANNEL_H__ 

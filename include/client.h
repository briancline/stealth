#ifndef __CLIENT_H__
#define __CLIENT_H__ "$Id: client.h,v 1.23 2002/04/03 03:58:58 bcline Exp $"

#include <string>
#include <map>
#include <ctime>
#include "access.h"
#include "types.h"
#include "flags.h"

class ChannelClient;


class Client
{
	public:

		Client()
			: c_modes( 0x0 ) {}
		Client( string ownerNum, string nick, int hops, int starttime, string ident, string host,
		        string modes, string ip, string num, string realname );
		~Client() {}
		

		string GetNick()
			{ return c_nick; }
			
		string GetUserName()
			{ return c_ident; }
			
		string GetHostName()
			{ return c_host; }
				
		string GetRealName()
			{ return c_realname; }
			
		string GetIP()
			{ return ipOct; }
			
		string GetIP64()
			{ return ipBase; }
			
		string GetNumeric()
			{ return c_num; }
		
		time_t GetConnectTS()
			{ return c_starttime; }

		modeType GetModes()
			{ return c_modes; }

		string GetConnectTime();

		void SetNick( string newNick )
			{ c_nick = newNick; }
		
		string GetModeString();
		bool IsOper();
		bool IsService();
		bool IsHostHidden();
		bool HasAccount();
		
		void AddMode( modeType whichMode );
		void AddMode( char whichMode );
		void RemMode( modeType whichMode );
		void RemMode( char whichMode );
		bool CheckMode( modeType whichMode );
			
		void SetAccount( string userName );
		string GetAccount();

		void SetHiddenHost();
		string GetHiddenHost();

		Access *GetAccessRecord();
		void SetAccessRecord( string userName );
		void ClearAccess();
		bool IsAuthed();
		bool IsSuspended();
		bool IsProtected();
		int GetLevel();

		string GetChanList();
		void AddChan( string chanName );
		void RemChan( string chanName );
		bool IsOnChan( string chanName );
		void OpOnAllChans();
		void RemAllChans();
		
	
	private:
		modeType c_modes;
		time_t c_starttime;
		
		string c_nick;
		string c_ident;
		string c_host;
		string c_realname;
			
		string ipOct;
		string ipBase;
		
		string c_num;
		
		Access *c_access;
		string c_account;
		string c_hiddenhost;

		map< string, ChannelClient*, less<string> > c_channels;
};


#endif // __CLIENT_H__

#ifndef __CHANCLIENT_H__
#define __CHANCLIENT_H__ "$Id: chanclient.h,v 1.3 2002/02/11 21:42:23 bcline Exp $"

#include <string>
#include "client.h"
#include "types.h"
#include "flags.h"


class ChannelClient
{
	public:

		ChannelClient();
		ChannelClient( Client *user );
		ChannelClient( Client *user, modeType &modes );
		ChannelClient( Client *user, string chanName, modeType &modes );

		bool IsOp();
		bool IsVoice();

		void SetOp();
		void SetVoice();

		void RemOp();
		void RemVoice();

		bool CheckMode( modeType mode );
		void SetMode( modeType mode );
		void RemMode( modeType mode );

		Client *GetClient();
		string GetNick();
		string GetUserName();
		string GetHostName();
		string GetNumeric();
		string GetIP();

		string GetChanName();
		void SetChanName( string chanName );


	private:

		Client *c_user;
		flagType c_modes;
		string c_cname;
};


#endif // __CHANCLIENT_H__ 

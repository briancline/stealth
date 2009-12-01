#ifndef __ACCESS_H__
#define __ACCESS_H__ "$Id: access.h,v 1.11 2002/05/28 04:49:41 bcline Exp $"

#include <list>
#include <string>
#include <sys/time.h>

using namespace std;


class Access
{
	public:

		Access();
		Access( string usrNick, int usrLevel, bool usrProtect, bool usrSuspend,
		        time_t usrLastSeen, string userPasswd, string usrMasks );
		~Access();
		
		string GetNick();
		string GetPass();
		int GetLevel();
		string GetMasks();
		string GetLastSeen();
		time_t GetLastTS();

		bool IsProtected();
		bool IsSuspended();
		bool IsValidHost( string fullHost );
		
		void SetNick( string nick );
		void SetLevel( int level );
		void SetProtect( bool protect );
		void SetSuspend( bool suspend );
		void SetTS( time_t lastSeen );
		void SetPassword( string pass );
		
		int AddHost( string hostmask );
		int DelHost( string hostmask );
		string GetHostList();
		

	private:

		string a_nick;
		string a_pass;
		int a_level;
		bool a_protect;
		bool a_suspend;
		time_t a_lastSeen;
		list<string> a_hosts;

};


#endif // __ACCESS_H__

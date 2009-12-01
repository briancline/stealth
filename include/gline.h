#ifndef __GLINE_H__
#define __GLINE_H__ "$Id: gline.h,v 1.10 2002/03/12 19:02:06 bcline Exp $"

#include <ctime>
#include <map>
#include <string>

using namespace std;


class Gline
{
	public:

		Gline() {};
		Gline( string host, time_t duration = 3600 );
		Gline( string host, time_t duration, string reason, string admin );
		Gline( string host, time_t duration, string admin );
		Gline( string host, string admin );
		Gline( string host );
		~Gline();

		void Register();

		string GetExpireTime();
		string GetSetTime();
		int FindNumHosts();

		time_t GetExpireTS()
			{ return ts_expire; }
		time_t GetSetTS()
			{ return ts_set; }
		string GetHost()
			{ return g_host; }
		string GetReason()
			{ return g_reason; }
		string GetOper()
			{ return g_oper; }
		int GetNumHosts()
			{ return g_numhosts; }


	private:

		string g_host;
		string g_reason;
		string g_oper;
		time_t ts_expire;
		time_t ts_set;
		int g_numhosts;

};


#endif // __GLINE_H__

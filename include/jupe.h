#ifndef __JUPE_H__
#define __JUPE_H__ "$Id: jupe.h,v 1.8 2002/04/23 01:33:06 bcline Exp $"

#include <list>
#include <sys/time.h>
#include "types.h"
#include "client.h"
#include "flags.h"


class Jupe
{
	public:

		Jupe() {}
		Jupe( string jupeName, time_t jupeDuration, string jupeDesc, bool active );
		Jupe( string jupeName, time_t jupeDuration, string jupeDesc, string jupeAdmin, bool active );
		Jupe( string jupeName, time_t jupeDuration, time_t jupeStart, string jupeDesc, bool active );
		Jupe( string jupeName, time_t jupeDuration, time_t jupeStart, string jupeDesc, string jupeAdmin, bool active );
		~Jupe() {}

		string GetName()
			{ return s_name; }

		string GetReason()
			{ return s_desc; }

		string GetAdmin()
			{ return s_admin; }

		time_t GetStartTS()
			{ return s_starttime; }

		time_t GetExpireTS()
			{ return s_expire; }

		time_t GetRemainingTime();

		string GetExpireString();

		void Register();
		void Activate();
		void Deactivate();

		bool IsActive()
			{ return s_active; }


	private:

		string s_name;
		string s_desc;
		string s_admin;

		time_t s_starttime;
		time_t s_duration;
		time_t s_expire;

		bool s_active;

};


#endif // __JUPE_H__

#ifndef __SERVER_H__
#define __SERVER_H__ "$Id: server.h,v 1.19 2002/02/18 16:36:21 bcline Exp $"

#include <list>
#include "types.h"
#include "client.h"
#include "flags.h"


class Server
{
	public:

		Server() : 
			s_flags( serverFlags::SERV_NONE )  {}
		Server( string uplinkNum, string serverName, int hops, time_t starttime, 
		        string serverNum, int maxconn, string desc, bool uplink );
		virtual ~Server() {}

		string GetName()
			{ return s_name; }

		string GetDesc()
			{ return s_desc; }

		bool GetStatus()
			{ return s_connected; }

		int GetMaxConn()
			{ return s_maxConnections; }

		int GetHopCount()
			{ return s_hopcount; }

		string GetNumeric()
			{ return s_num; }

		int GetNumericInt()
			{ return s_intNum; }

		string GetUplinkNumeric()
			{ return s_ownernum; }

		time_t GetStartTime()
			{ return s_starttime; }

		bool IsUplink();

		void AddFlag( flagType whichFlag );
		void RemFlag( flagType whichFlag );
		bool CheckFlag( flagType whichFlag );

		void NumericSet( string num );
		void NumericInc();
		int NumericCount();


	private:

		flagType s_flags; 
		
		string s_name;
		string s_desc;

		bool s_connected;
		int s_maxConnections;
		int s_hopcount;
		time_t s_starttime;

		string s_num;
		string s_ownernum;
		int s_intNum;

		int s_numCount;

		//list<ServClient> s_users;
};


#endif // __SERVER_H__

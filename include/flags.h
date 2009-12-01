#ifndef __FLAGS_H__
#define __FLAGS_H__ "$Id: flags.h,v 1.10 2002/02/16 02:00:10 bcline Exp $"

#include "types.h"


	namespace serverFlags
	{
		const flagType SERV_NONE      = 0x000;   // no flags YET
		const flagType SERV_UPLINK    = 0x001;   // Is this our uplink?
		const flagType SERV_BURST     = 0x002;   // has sent EOB
		const flagType SERV_BURST_ACK = 0x004;   // has sent EOB_ACK
		const flagType SERV_SERVICE   = 0x008;   // includes "service" || "channels" || "uworld"
		const flagType SERV_PING      = 0x010;   // waiting for PING response ( uplink server only )
	}

	namespace clientFlags
	{
		const modeType USER_NONE      = 0x000;   // -
		const modeType USER_OPER      = 0x001;   // o
		const modeType USER_WALLOPS   = 0x002;   // w
		const modeType USER_INVISIBLE = 0x004;   // i
		const modeType USER_DEAF      = 0x008;   // d
		const modeType USER_SERVICE   = 0x010;   // k
		const modeType USER_GODMODE   = 0x020;   // h
		const modeType USER_HACKS     = 0x040;   // g
		const modeType USER_ACCOUNT   = 0x080;   // r
		const modeType USER_HIDEHOST  = 0x100;   // x
	}

	namespace channelFlags
	{
		const modeType CHAN_NONE      = 0x000;   // -
		const modeType CHAN_INVITE    = 0x001;   // i
		const modeType CHAN_KEY       = 0x002;   // k
		const modeType CHAN_LIMIT     = 0x004;   // l
		const modeType CHAN_MODERATED = 0x008;   // m
		const modeType CHAN_NOEXMSGS  = 0x010;   // n
		const modeType CHAN_PRIVATE   = 0x020;   // p
		const modeType CHAN_SECRET    = 0x040;   // s
		const modeType CHAN_TOPICOP   = 0x080;   // t
	}

	namespace chanClientFlags
	{
		const modeType CHAN_NONE      = 0x000;
		const modeType CHAN_OP        = 0x001;
		const modeType CHAN_VOICE     = 0x002;
	}


#endif // __FLAGS_H__

#ifndef __EVENTS_H__
#define __EVENTS_H__ "$Id: events.h,v 1.5 2002/06/24 22:45:10 bcline Exp $"

#include <string>


enum EventSource { EVT_CLIENT, EVT_SERVER };

enum ServerEventType
{
	SRV_AWAY,
	SRV_ACCOUNT,
	SRV_ADMIN,
	SRV_BURST,
	SRV_CREATE,
	SRV_CLEARMODE,
	SRV_KILL,
	SRV_DESTRUCT,
	SRV_DESYNCH,
	SRV_EOBACK,
	SRV_EOB,
	SRV_ERROR,
	SRV_PING,
	SRV_GLINE,
	SRV_INVITE,
	SRV_JOIN,
	SRV_KICK,
	SRV_JUPE,
	SRV_LEAVE,
	SRV_UMODE,
	SRV_CHMODE,
	SRV_NICK,
	SRV_NICKCHG,
	SRV_QUIT,
	SRV_STATS,
	SRV_SERVER,
	SRV_SQUIT,
	SRV_TOPIC,
	SRV_SILENCE,
	SRV_VERSION,
	SRV_WHOIS,
	SRV_WALLOPS,
	SRV_WALLUSERS,
	SRV_END_OF_MAP
};

enum CommandEventType 
{
	CMD_QUOTE,
	CMD_DIE,
	CMD_DISABLE,
	CMD_ENABLE,
	CMD_RESTART,
	CMD_CHLEVEL,
	CMD_CHNAME,
	CMD_SET,
	CMD_WRITE,
	CMD_ADDUSER,
	CMD_MODUSER,
	CMD_REMUSER,
	CMD_BROADCAST,
	CMD_CLONE,
	CMD_CLONEACT,
	CMD_CLONEJOIN,
	CMD_CLONEPART,
	CMD_CLONEQUIT,
	CMD_CLONESAY,
	CMD_REOP,
	CMD_SETTIME,
	CMD_JUPE,
	CMD_REFRESHG,
	CMD_UNJUPE,
	CMD_CLEARCHAN,
	CMD_DEOPALL,
	CMD_DEVOICEALL,
	CMD_OPALL,
	CMD_VOICEALL,
	CMD_BAN,
	CMD_GLINE,
	CMD_INVITE,
	CMD_INVITEME,
	CMD_MODE,
	CMD_REMGLINE,
	CMD_UNBAN,
	CMD_DEOP,
	CMD_DEVOICE,
	CMD_KICK,
	CMD_OP,
	CMD_VOICE,
	CMD_CHANINFO,
	CMD_CHANLIST,
	CMD_HELP,
	CMD_LOGIN,
	CMD_LOGOUT,
	CMD_MAP,
	CMD_OPERINFO,
	CMD_OPERMSG,
	CMD_SCAN,
	CMD_SERVINFO,
	CMD_SHOW,
	CMD_SHOWCOMMANDS,
	CMD_TRANSLATE,
	CMD_UPTIME,
	CMD_WHOIS,
	CMD_WHOISON,
	CMD_CTCP_PING,
	CMD_CTCP_VERSION,
	CMD_CTCP_GENDER,
	CMD_END_OF_MAP
};

struct ServerEvent
{
	bool report;
	ServerEventType event;
	EventSource source;
	char *token;
	char *format;
};

struct CommandEvent
{
	bool report;
	CommandEventType event;
	char *token;
	char *format;
};

extern ServerEvent serverEvents[];
extern CommandEvent commandEvents[];

#endif // __EVENTS_H__

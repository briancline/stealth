#include "cmdmap.h"


PrivMsgTabItem PrivMsgTab[] =
{
	{  "QUOTE"           ,  "QUOTE"           ,  "<protocol>"                          ,  1002 ,   1 ,  1 , cmd_QUOTE         },
	{  "DIE"             ,  "DIE"             ,  " "                                   ,  1000 ,   0 ,  1 , cmd_DIE           },
	{  "DISABLE"         ,  "DISABLE"         ,  "<command>"                           ,  1000 ,   1 ,  1 , cmd_DISABLE       },
	{  "ENABLE"          ,  "ENABLE"          ,  "<command>"                           ,  1000 ,   1 ,  1 , cmd_ENABLE        },
	{  "RESTART"         ,  "RESTART"         ,  " "                                   ,  1000 ,   0 ,  1 , cmd_RESTART       },
	{  "CHLEVEL"         ,  "CHLEVEL"         ,  "<command name> <new level>"          ,   900 ,   2 ,  1 , cmd_CHLEVEL       },
	{  "CHNAME"          ,  "CHNAME"          ,  "<command name> <new command name>"   ,   900 ,   2 ,  1 , cmd_CHNAME        },
	{  "SET"             ,  "SET"             ,  "<param> <value>"                     ,   900 ,   2 ,  1 , cmd_SET           },
	{  "WRITE"           ,  "WRITE"           ,  " "                                   ,   900 ,   0 ,  1 , cmd_WRITE         },
	{  "ADDUSER"         ,  "ADDUSER"         ,  "<nick> <level> <password> <masks>"   ,   800 ,   4 ,  1 , cmd_ADDUSER       },
	{  "MODUSER"         ,  "MODUSER"         ,  "<nick> <param> <value>"              ,   800 ,   3 ,  1 , cmd_MODUSER       },
	{  "REMUSER"         ,  "REMUSER"         ,  "<nick>"                              ,   800 ,   1 ,  1 , cmd_REMUSER       },
	{  "BROADCAST"       ,  "BROADCAST"       ,  "<message>"                           ,   700 ,   1 ,  1 , cmd_BROADCAST     },
	{  "CLONE"           ,  "CLONE"           ,  "<nick> <ident> <host> <realname>"    ,   700 ,   4 ,  1 , cmd_CLONE         },
	{  "CLONEACT"        ,  "CLONEACT"        ,  "<clone nick> <channel> <text>"       ,   700 ,   3 ,  1 , cmd_CLONEACT      },
	{  "CLONEJOIN"       ,  "CLONEJOIN"       ,  "<clone nick> <channel>"              ,   700 ,   2 ,  1 , cmd_CLONEJOIN     },
	{  "CLONEPART"       ,  "CLONEPART"       ,  "<clone nick> <channel>"              ,   700 ,   2 ,  1 , cmd_CLONEPART     },
	{  "CLONEQUIT"       ,  "CLONEQUIT"       ,  "<clone nick> <quit message>"         ,   700 ,   2 ,  1 , cmd_CLONEQUIT     },
	{  "CLONESAY"        ,  "CLONESAY"        ,  "<clone nick> <channel> <text>"       ,   700 ,   3 ,  1 , cmd_CLONESAY      },
	{  "REOP"            ,  "REOP"            ,  "<nick>"                              ,   700 ,   1 ,  1 , cmd_REOP          },
	{  "SETTIME"         ,  "SETTIME"         ,  " "                                   ,   700 ,   0 ,  1 , cmd_SETTIME       },
	{  "JUPE"            ,  "JUPE"            ,  "<servername> <duration> <reason>"    ,   500 ,   3 ,  1 , cmd_JUPE          },
	{  "REFRESHG"        ,  "REFRESHG"        ,  " "                                   ,   500 ,   0 ,  1 , cmd_REFRESHG      },
	{  "UNJUPE"          ,  "UNJUPE"          ,  "<servername>"                        ,   500 ,   1 ,  1 , cmd_UNJUPE        },
	{  "CLEARCHAN"       ,  "CLEARCHAN"       ,  "<channel> <parameters>"              ,   300 ,   2 ,  1 , cmd_CLEARCHAN     },
	{  "DEOPALL"         ,  "DEOPALL"         ,  "<channel>"                           ,   300 ,   1 ,  1 , cmd_DEOPALL       },
	{  "DEVOICEALL"      ,  "DEVOICEALL"      ,  "<channel>"                           ,   300 ,   1 ,  1 , cmd_DEVOICEALL    },
	{  "OPALL"           ,  "OPALL"           ,  "<channel>"                           ,   300 ,   1 ,  1 , cmd_OPALL         },
	{  "VOICEALL"        ,  "VOICEALL"        ,  "<channel>"                           ,   300 ,   1 ,  1 , cmd_VOICEALL      },
	{  "BAN"             ,  "BAN"             ,  "<channel> <mask>"                    ,   200 ,   2 ,  1 , cmd_BAN           },
	{  "GLINE"           ,  "GLINE"           ,  "<mask> <duration> <reason>"          ,   200 ,   3 ,  1 , cmd_GLINE         },
	{  "INVITE"          ,  "INVITE"          ,  "<channel>"                           ,   200 ,   1 ,  1 , cmd_INVITE        },
	{  "INVITEME"        ,  "INVITEME"        ,  " "                                   ,   300 ,   0 ,  1 , cmd_INVITEME      },
	{  "MODE"            ,  "MODE"            ,  "<channel> <modes> [params]"          ,   200 ,   2 ,  1 , cmd_MODE          },
	{  "REMGLINE"        ,  "REMGLINE"        ,  "<mask>"                              ,   200 ,   1 ,  1 , cmd_REMGLINE      },
	{  "UNBAN"           ,  "UNBAN"           ,  "<channel> <mask>"                    ,   200 ,   2 ,  1 , cmd_UNBAN         },
	{  "DEOP"            ,  "DEOP"            ,  "<channel> [nick1 nick2 nick3 ...]"   ,   100 ,   1 ,  1 , cmd_DEOP          },
	{  "DEVOICE"         ,  "DEVOICE"         ,  "<channel> <nick> [nick2 nick3 ...]"  ,   100 ,   2 ,  1 , cmd_DEVOICE       },
	{  "KICK"            ,  "KICK"            ,  "<channel> <nick> <reason>"           ,   100 ,   3 ,  1 , cmd_KICK          },
	{  "OP"              ,  "OP"              ,  "<channel> [nick1 nick2 nick3 ...]"   ,   100 ,   1 ,  1 , cmd_OP            },
	{  "VOICE"           ,  "VOICE"           ,  "<channel> <nick> [nick2 nick3 ...]"  ,   100 ,   2 ,  1 , cmd_VOICE         },
	{  "CHANINFO"        ,  "CHANINFO"        ,  "<channel>"                           ,     0 ,   1 ,  1 , cmd_CHANINFO      },
	{  "CHANLIST"        ,  "CHANLIST"        ,  "<search mask>"                       ,     0 ,   1 ,  1 , cmd_CHANLIST      },
	{  "HELP"            ,  "HELP"            ,  "[command]"                           ,     0 ,   0 ,  1 , cmd_HELP          },
	{  "LOGIN"           ,  "LOGIN"           ,  "<username> <password>"               ,     0 ,   2 ,  1 , cmd_LOGIN         },
	{  "LOGOUT"          ,  "LOGOUT"          ,  " "                                   ,     0 ,   0 ,  1 , cmd_LOGOUT        },
	{  "MAP"             ,  "MAP"             ,  " "                                   ,     0 ,   0 ,  1 , cmd_MAP           },
	{  "OPERINFO"        ,  "OPERINFO"        ,  "<username>"                          ,     0 ,   1 ,  1 , cmd_OPERINFO      },
	{  "OPERMSG"         ,  "OPERMSG"         ,  "<message>"                           ,     0 ,   1 ,  1 , cmd_OPERMSG       },
	{  "SCAN"            ,  "SCAN"            ,  "<hostmask>"                          ,     0 ,   1 ,  1 , cmd_SCAN          },
	{  "SERVINFO"        ,  "SERVINFO"        ,  "<server name>"                       ,     0 ,   1 ,  1 , cmd_SERVINFO      },
	{  "SHOW"            ,  "SHOW"            ,  "<param> [arguments]"                 ,     0 ,   1 ,  1 , cmd_SHOW          },
	{  "SHOWCOMMANDS"    ,  "SHOWCOMMANDS"    ,  " "                                   ,     0 ,   0 ,  1 , cmd_SHOWCOMMANDS  },
	{  "TRANSLATE"       ,  "TRANSLATE"       ,  "<numeric>"                           ,     0 ,   1 ,  1 , cmd_TRANSLATE     },
	{  "UPTIME"          ,  "UPTIME"          ,  " "                                   ,     0 ,   0 ,  1 , cmd_UPTIME        },
	{  "WHOIS"           ,  "WHOIS"           ,  "<nick>"                              ,     0 ,   1 ,  1 , cmd_WHOIS         },
	{  "WHOISON"         ,  "WHOISON"         ,  "<channel>"                           ,     0 ,   1 ,  1 , cmd_WHOISON       },
	{  "\001PING"        ,  "\001PING"        ,  "<timestamp>"                         ,    -1 ,   1 ,  1 , cmd_CTCP_PING     },
	{  "\001VERSION\001" ,  "\001VERSION\001" ,  " "                                   ,    -1 ,   0 ,  1 , cmd_CTCP_VERSION  },
	{  "\001GENDER\001"  ,  "\001GENDER\001"  ,  " "                                   ,    -1 ,   0 ,  1 , cmd_CTCP_GENDER   },
	{  "-"               ,  "-"               ,  "-"                                   ,     0 ,   0 ,  0 , NULL              }
};


#ifndef __CMDMAP_H__
#define __CMDMAP_H__ "$Id: cmdmap.h,v 1.26 2002/06/24 22:45:10 bcline Exp $"

#include <string>
#include "stealth.h"
#include "token.h"
#include "types.h"

#define addCommandProto( cmdName ) \
	cmdStatusType cmd_##cmdName( Numeric nSrc, Numeric nDst, Token tokens )

struct PrivMsgTabItem
{
	string CmdName;
	string CmdPubName;
	string CmdSyntax;
	int CmdLevel;
	int CmdParams;
	bool CmdEnabled;
	cmdStatusType (*Exec) ( string, string, Token );
};


addCommandProto( QUOTE         );
addCommandProto( DIE           );
addCommandProto( DISABLE       );
addCommandProto( ENABLE        );
addCommandProto( RESTART       );
addCommandProto( CHLEVEL       );
addCommandProto( CHNAME        );
addCommandProto( SET           );
addCommandProto( WRITE         );
addCommandProto( ADDUSER       );
addCommandProto( MODUSER       );
addCommandProto( REMUSER       );
addCommandProto( BROADCAST     );
addCommandProto( CLONE         );
addCommandProto( CLONEACT      );
addCommandProto( CLONEJOIN     );
addCommandProto( CLONEPART     );
addCommandProto( CLONEQUIT     );
addCommandProto( CLONESAY      );
addCommandProto( REOP          );
addCommandProto( SETTIME       );
addCommandProto( JUPE          );
addCommandProto( REFRESHG      );
addCommandProto( UNJUPE        );
addCommandProto( CLEARCHAN     );
addCommandProto( DEOPALL       );
addCommandProto( DEVOICEALL    );
addCommandProto( OPALL         );
addCommandProto( VOICEALL      );
addCommandProto( BAN           );
addCommandProto( GLINE         );
addCommandProto( INVITE        );
addCommandProto( INVITEME      );
addCommandProto( MODE          );
addCommandProto( REMGLINE      );
addCommandProto( UNBAN         );
addCommandProto( DEOP          );
addCommandProto( DEVOICE       );
addCommandProto( KICK          );
addCommandProto( OP            );
addCommandProto( VOICE         );
addCommandProto( CHANINFO      );
addCommandProto( CHANLIST      );
addCommandProto( HELP          );
addCommandProto( LOGIN         );
addCommandProto( LOGOUT        );
addCommandProto( MAP           );
addCommandProto( OPERINFO      );
addCommandProto( OPERMSG       );
addCommandProto( SCAN          );
addCommandProto( SERVINFO      );
addCommandProto( SHOW          );
addCommandProto( SHOWCOMMANDS  );
addCommandProto( TRANSLATE     );
addCommandProto( UPTIME        );
addCommandProto( WHOIS         );
addCommandProto( WHOISON       );
addCommandProto( CTCP_PING     );
addCommandProto( CTCP_VERSION  );
addCommandProto( CTCP_GENDER   );


extern PrivMsgTabItem PrivMsgTab[];

#endif // __CMDMAP_H__

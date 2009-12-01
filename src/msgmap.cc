#include "msgmap.h"

ServMsgTabItem ServMsgTab[] =
{
	{ "A"       , msg_A      },	// away
	{ "AC"      , msg_AC     },	// account
	{ "AD"      , msg_AD     },	// admin
	{ "B"       , msg_B      },	// burst
	{ "C"       , msg_C      },	// create
	{ "CM"      , msg_CM     },	// clearmode
	{ "D"       , msg_D      },	// kill
	{ "DE"      , msg_DE     },	// destruct
	{ "DS"      , msg_DS     },	// desynch
	{ "EA"      , msg_EA     },	// end of burst acknowledgement
	{ "EB"      , msg_EB     },	// end of burst
	{ "ERROR"   , msg_ERROR  },	// uplink error
	{ "G"       , msg_G      },	// ping
	{ "GL"      , msg_GL     },	// gline
	{ "I"       , msg_I      },	// invite
	{ "J"       , msg_J      },	// join
	{ "K"       , msg_K      },	// kick
	{ "JU"      , msg_JU     },	// jupe
	{ "L"       , msg_L      },	// leave
	{ "P"       , msg_P      },	// message (chan and private)
	{ "M"       , msg_M      },	// mode
	{ "N"       , msg_N      },	// nick
	{ "O"       , msg_P      },	// notice
	{ "OM"      , msg_M      },	// opermode
	{ "Q"       , msg_Q      },	// quit
	{ "PASS"    , msg_PASS   },	// uplink pass
	{ "R"       , msg_R      },	// stats
	{ "S"       , msg_S      },	// server
	{ "SERVER"  , msg_SERVER },	// uplink server
	{ "SQ"      , msg_SQ     },	// squit
	{ "T"       , msg_T      },	// topic
	{ "U"       , msg_U      },	// silence
	{ "V"       , msg_V      },	// version
	{ "W"       , msg_W      },	// whois
	{ "WA"      , msg_WA     },	// wallops
	{ "WU"      , msg_WU     },	// wallusers
	{ "-"       , NULL       }
};


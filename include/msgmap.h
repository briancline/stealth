#ifndef __MSGMAP_H__
#define __MSGMAP_H__ "$Id: msgmap.h,v 1.18 2002/03/04 03:24:16 bcline Exp $"

#include <string>
#include "stealth.h"
#include "types.h"
#include "token.h"

#define addMessageProto( funcSuffix ) \
	msgStatusType msg_##funcSuffix ( Numeric nSrc, Numeric nDst, Token tokens );

struct ServMsgTabItem
{
	string ServMsg;
	msgStatusType (*Exec) ( string, string, Token );
};


addMessageProto( A      );
addMessageProto( AC     );
addMessageProto( AD     );
addMessageProto( B      );
addMessageProto( C      );
addMessageProto( CM     );
addMessageProto( D      );
addMessageProto( DE     );
addMessageProto( DS     );
addMessageProto( EA     );
addMessageProto( EB     );
addMessageProto( ERROR  );
addMessageProto( G      );
addMessageProto( GL     );
addMessageProto( I      );
addMessageProto( J      );
addMessageProto( K      );
addMessageProto( JU     );
addMessageProto( L      );
addMessageProto( P      );
addMessageProto( M      );
addMessageProto( N      );
addMessageProto( Q      );
addMessageProto( PASS   );
addMessageProto( R      );
addMessageProto( S      );
addMessageProto( SERVER );
addMessageProto( SQ     );
addMessageProto( T      );
addMessageProto( U      );
addMessageProto( V      );
addMessageProto( W      );
addMessageProto( WA     );
addMessageProto( WU     );


extern ServMsgTabItem ServMsgTab[];

#endif // __MSGMAP_H__

#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_CHANLIST ( Numeric nSrc, Numeric nDst, Token tokens )
{
	ChannelMapType::iterator chan;
	string chanMask = tokens[4], chanName;
	int count = 0;

	Net->Send( "%s O %s :  [ %-20s ] [ %8s ] [ %5s ]\n", nDst.c_str(), nSrc.c_str(),
		"CHANNEL NAME", "MODES", "COUNT", "CURRENT CHANNEL TOPIC" );

	for( chan = Net->ChannelIter(); chan != Net->ChannelTail(); chan++ )
	{
		chanName = chan->second->GetName();
		chanName = chanName.substr( 1, chanName.length() );

		if( match( chanMask, chanName ) )
		{
			Net->Send( "%s O %s :  [ %-20s ] [ %8s ] [ %5d ]\n", nDst.c_str(), nSrc.c_str(),
				chan->second->GetName().c_str(), chan->second->GetModeString().c_str(),
				chan->second->UserCount() ); 
			count++;
		}
	}

	Net->Send( "%s O %s :\002%d matches found.\002\n", nDst.c_str(), nSrc.c_str(), count );

	Report( CMD_CHANLIST, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


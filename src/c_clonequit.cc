#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_CLONEQUIT ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Client *user = Net->FindClientByNick( tokens[4] );
	string text = tokens.Assemble( 6 );

	if( user == NULL )
	{
		Net->Send( "%s O %s :Clone %s does not exist.\n", nDst.c_str(), nSrc.c_str(),
			tokens[4].c_str() );
		return CMD_ERROR;
	}

	Net->Send( "%s Q :%s\n", user->GetNumeric().c_str(), text.c_str() );
	Net->RemClient( user->GetNumeric() );
	
	Report( CMD_CLONEQUIT, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


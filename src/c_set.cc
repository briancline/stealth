#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_SET ( Numeric nSrc, Numeric nDst, Token tokens )
{
	string paramName = tokens[4], newValue = tokens.Assemble( 6 );

	paramName = strUpper( paramName );

	Net->SetParameter( paramName, newValue );
	Net->Send( "%s O %s :Successfully set \"%s\" to \"%s\".\n", nDst.c_str(), nSrc.c_str(),
		paramName.c_str(), Net->GetConfig( paramName ) );
	
	Report( CMD_SET, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}

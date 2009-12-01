#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_SERVINFO ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Server *serv;
	ServerMapType::iterator iter;
	string mask = tokens[4];
	int count = 0;

	for( iter = Net->ServIter(); iter != Net->ServTail(); iter++ )
	{
		serv = iter->second;

		if( match( mask, serv->GetName() ) )
		{
			if( count > 0 )
				Net->Send( "%s O %s : \n", nDst.c_str(), nSrc.c_str() );

			Net->Send( "%s O %s :Server \002%s\002:\n", nDst.c_str(), nSrc.c_str(), serv->GetName().c_str() );
			Net->Send( "%s O %s :  Numeric:      %s (%d)\n", nDst.c_str(), nSrc.c_str(), serv->GetNumeric().c_str(),
				serv->GetNumericInt() );
			Net->Send( "%s O %s :  Next Numeric: %s%s (%d)\n", nDst.c_str(), nSrc.c_str(), 
				serv->GetNumeric().c_str(), IntToBase64( serv->NumericCount()+1, 3 ).c_str(), 
				serv->NumericCount()+1 );
			Net->Send( "%s O %s :  Description:  %s\n", nDst.c_str(), nSrc.c_str(), serv->GetDesc().c_str() );
			Net->Send( "%s O %s :  Max Users:    %d\n", nDst.c_str(), nSrc.c_str(), serv->GetMaxConn() );
			count++;
		}
	}

	if( count == 0 )
	{
		Net->Send( "%s O %s :No servers matched your query.\n", nDst.c_str(), nSrc.c_str() );
		return CMD_ERROR;
	}

	Report( CMD_SERVINFO, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}

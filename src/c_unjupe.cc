#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_UNJUPE ( Numeric nSrc, Numeric nDst, Token tokens )
{
	bool isMask = false;
	int count = 0;
	string servName = tokens[4], mask;
	JupeMapType::iterator iter;
	Jupe *jupe;

	if( servName.find( '*' ) != string::npos || servName.find( '?' ) != string::npos )
		isMask = true;

	if( isMask )
	{
		mask = servName;

		for( iter = Net->JupeIter(); iter != Net->JupeTail(); iter++ )
		{
			jupe = iter->second;
			if( match( mask, jupe->GetName() ) )
			{
				servName = jupe->GetName();
				
				if( jupe->GetExpireTS() <= time( NULL ) )
				{
					Net->RemJupe( servName );
					Net->Send( "%s O %s :Removed jupe for %s.\n", nDst.c_str(), nSrc.c_str(),
						servName.c_str() );
				}
				else
				{
					jupe->Deactivate();
					Net->Send( "%s O %s :Deactivated jupe for %s.\n", nDst.c_str(), nSrc.c_str(),
						servName.c_str() );
				}
				
				count++;
			}
		}

		if( count == 0 )
		{
			Net->Send( "%s O %s :No jupes matched %s.\n", nDst.c_str(), nSrc.c_str(),
				servName.c_str() );
			return CMD_ERROR;
		}
	}
	else
	{
		jupe = Net->FindJupeByName( servName );

		if( jupe == NULL )
		{
			Net->Send( "%s O %s :No jupe for %s found.\n", nDst.c_str(), nSrc.c_str(),
				servName.c_str() );
			return CMD_ERROR;
		}

		if( jupe->GetExpireTS() <= time( NULL ) )
		{
			Net->RemJupe( servName );
			Net->Send( "%s O %s :Removed jupe for %s.\n", nDst.c_str(), nSrc.c_str(),
				servName.c_str() );
		}
		else
		{
			jupe->Deactivate();
			Net->Send( "%s O %s :Deactivated jupe for %s.\n", nDst.c_str(), nSrc.c_str(),
				servName.c_str() );
		}
	}
		
	Report( CMD_UNJUPE, nSrc, 0, tokens.Assemble( 5 ).c_str() );
	
	return CMD_SUCCESS;
}


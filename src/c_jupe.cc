#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_JUPE ( Numeric nSrc, Numeric nDst, Token tokens )
{
	int count = 0;
	bool isMask = false;
	time_t duration = atol( tokens[5].c_str() );
	string reason = tokens.Assemble( 7 ), servName = tokens[4], strNum, mask;
	ServerMapType::iterator iter;
	Client *user = Net->FindClientByNum( nSrc );
	Server *serv;
	Jupe *jupe;

	if( servName.find( '*' ) != string::npos || servName.find( '?' ) != string::npos )
		isMask = true;
		
	if( isMask == true )
	{
		mask = servName;
		
		for( iter = Net->ServIter(); iter != Net->ServTail(); iter++ )
		{
			serv = iter->second;

			if( match( mask, serv->GetName() ) )
			{
				servName = serv->GetName();
				jupe = Net->FindJupeByName( serv->GetName() );

				if( jupe != NULL )
				{
					if( jupe->IsActive() )
					{
						Net->Send( "%s O %s :Not juping server %s (already juped).\n", nDst.c_str(), nSrc.c_str(), 
							jupe->GetName().c_str() );
						continue;
					}
					else
					{
						Net->Send( "%s O %s :Re-activating jupe %s.\n", nDst.c_str(), nSrc.c_str(), 
							jupe->GetName().c_str() );
						jupe->Activate();
						continue;
					}
				}
				else if( serv->IsUplink() )
				{
					Net->Send( "%s O %s :Not juping server %s (cannot jupe uplink).\n", nDst.c_str(),
						nSrc.c_str(), serv->GetName().c_str() );
					continue;
				}
				else if( serv->GetName() == Net->GetConfig( "SERVNAME" ) )
				{
					Net->Send( "%s O %s :Not juping server %s (cannot jupe self).\n", nDst.c_str(),
						nSrc.c_str(), serv->GetName().c_str() );
					continue;
				}

				Net->RemServer( serv->GetNumeric() );
				iter--;

				jupe = Net->AddJupe( servName, duration, time( NULL ), reason, user->GetAccount(), true );
				jupe->Register();

				Net->Send( "%s O %s :Successfully juped %s, expiring at %s.\n", nDst.c_str(),
					nSrc.c_str(), jupe->GetName().c_str(), jupe->GetExpireString().c_str() );

				count++;
			}
		}
		
		if( count == 0 )
		{
			Net->Send( "%s O %s :No servers were found matching the mask %s.\n", nDst.c_str(),
				nSrc.c_str(), mask.c_str() );
			return CMD_ERROR;
		}
	}
	else
	{
		serv = Net->FindServerByName( servName );

		if( serv != NULL )
		{
			if( serv->IsUplink() )
			{
				Net->Send( "%s O %s :Cannot jupe my own uplink!\n", nDst.c_str(), nSrc.c_str() );
				return CMD_ERROR;
			}

			Net->RemServer( serv->GetNumeric() );
		}

		jupe = Net->FindJupeByName( servName );

		if( jupe != NULL )
		{
			if( jupe->IsActive() )
			{
				Net->Send( "%s O %s :Jupe %s already exists.\n", nDst.c_str(), nSrc.c_str(), servName.c_str() );
				return CMD_ERROR;
			}
			else
			{
				Net->Send( "%s O %s :Re-activating jupe %s.\n", nDst.c_str(), nSrc.c_str(), servName.c_str() );
				jupe->Activate();
				return CMD_SUCCESS;
			}
		}

		jupe = Net->AddJupe( servName, duration, time( NULL ), reason, user->GetAccount(), true );
		jupe->Register();

		Net->Send( "%s O %s :Successfully juped %s, expiring at %s.\n", nDst.c_str(),
			nSrc.c_str(), jupe->GetName().c_str(), jupe->GetExpireString().c_str() );
	}
	
	Report( CMD_JUPE, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


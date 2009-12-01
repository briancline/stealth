#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_MODUSER ( Numeric nSrc, Numeric nDst, Token tokens )
{
	string nick = tokens[4], param = tokens[5], value = tokens[6], toggle;
	Client *user = Net->FindClientByNum( nSrc );
	Access *rec = Net->FindAccessEntry( nick ), *newrec;
	int uLevel = user->GetLevel(), newLevel = 0, oldLevel = 0;

	if( rec == NULL )
	{
		Net->Send( "%s O %s :User %s is not in the access list.\n", nDst.c_str(), nSrc.c_str(), nick.c_str() );
		return CMD_ERROR;
	}

	param = strUpper( param );

	if( param != "PASSWORD" && param != "ADDMASK" && param != "DELMASK" && 
			uLevel <= rec->GetLevel() )
	{
		Net->Send( "%s O %s :You may not modify someone with a level greater than or equal to your own.\n",
			nDst.c_str(), nSrc.c_str() );
		return CMD_ERROR;
	}

	if( param == "PASSWORD" )
	{
		rec->SetPassword( value );
		Net->Send( "%s O %s :Successfully changed password for %s to %s.\n", nDst.c_str(), nSrc.c_str(),
			rec->GetNick().c_str(), rec->GetPass().c_str() );
	}
	else if( param == "LEVEL" )
	{
		newLevel = atoi( value.c_str() );
		oldLevel = rec->GetLevel();

		if( newLevel >= uLevel )
		{
			Net->Send( "%s O %s :You may not make a user's level greater than or equal to your own.\n",
				nDst.c_str(), nSrc.c_str() );
			return CMD_ERROR;
		}

		rec->SetLevel( newLevel );

		Net->Send( "%s O %s :Successfully changed level for %s from %d to %d.\n", nDst.c_str(), nSrc.c_str(),
			rec->GetNick().c_str(), oldLevel, newLevel );
	}
	else if( param == "ADDHOST" )
	{
		if( rec->AddHost( value ) == 1 )
		{
			Net->Send( "%s O %s :Host %s already exists or is already covered by an existing mask.\n", nDst.c_str(),
				nSrc.c_str(), value.c_str() );
			return CMD_ERROR;
		}
		Net->Send( "%s O %s :Successfully added host %s for user %s.\n", nDst.c_str(), nSrc.c_str(), 
			value.c_str(), rec->GetNick().c_str() );
	}
	else if( param == "REMHOST" )
	{
		if( rec->DelHost( value ) == 1 )
		{
			Net->Send( "%s O %s :Host %s does not exist for user %s.\n", nDst.c_str(), nSrc.c_str(), 
				value.c_str(), rec->GetNick().c_str() );
			return CMD_ERROR;
		}
		Net->Send( "%s O %s :Successfully removed host %s for user %s.\n", nDst.c_str(), nSrc.c_str(), 
			value.c_str(), rec->GetNick().c_str() );
	}
	else if( param == "SUSPEND" )
	{
		toggle = strLower( value );

		if( toggle == "off" )
		{
			rec->SetSuspend( 0 );
			Net->Send( "%s O %s :Set suspend flag to OFF for %s.\n", nDst.c_str(), nSrc.c_str(), 
				rec->GetNick().c_str() );
		}
		else if( toggle == "on" )
		{
			rec->SetSuspend( 1 );
			Net->Send( "%s O %s :Set suspend flag to ON for %s.\n", nDst.c_str(), nSrc.c_str(), 
				rec->GetNick().c_str() );
		}
		else
		{
			Net->Send( "%s O %s :Suspend flag must be ON or OFF.\n", nDst.c_str(), nSrc.c_str() );
			return CMD_ERROR;
		}
	}
	else if( param == "PROTECT" )
	{
		toggle = strLower( value );

		if( toggle == "off" )
		{
			rec->SetProtect( false );
			Net->Send( "%s O %s :Set protect flag to OFF for %s.\n", nDst.c_str(), nSrc.c_str(),
				rec->GetNick().c_str() );
		}
		else if( toggle == "on" )
		{
			rec->SetProtect( true );
			Net->Send( "%s O %s :Set protect flag to ON for %s.\n", nDst.c_str(), nSrc.c_str(),
				rec->GetNick().c_str() );
		}
		else
		{
			Net->Send( "%s O %s :Protect flag must be ON or OFF.\n", nDst.c_str(), nSrc.c_str() );
			return CMD_ERROR;
		}
	}
	else if( param == "NICK" )
	{
		newrec = Net->FindAccessEntry( value );

		if( newrec != NULL )
		{
			Net->Send( "%s O %s :A user with nick %s already exists in the access list.\n", nDst.c_str(),
				nSrc.c_str(), newrec->GetNick().c_str() );
			return CMD_ERROR;
		}

		rec->SetNick( value );
	}
	else
	{
		Net->Send( "%s O %s :Invalid option.\n", nDst.c_str(), nSrc.c_str() );
		return CMD_ERROR;
	}

	Net->SaveData();

	Report( CMD_MODUSER, nSrc, 0, tokens.Assemble( 5 ).c_str() );
	
	return CMD_SUCCESS;
}

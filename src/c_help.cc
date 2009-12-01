#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_HELP ( Numeric nSrc, Numeric nDst, Token tokens )
{
	int i = 0;
	string command, fileName, strLine;
	bool valid = false;
	ifstream helpFile;
	string::size_type nPos, sPos, lPos;
	Client *bot = Net->FindClientByNum( nDst );

	if( tokens.numTokens() >= 5 )
		command = tokens[4];
	else
		command = "HELP";
	
	command = strUpper( command );

	for( i = 0; PrivMsgTab[i].CmdPubName != "-"; i++ )
	{
		if( strCmp( PrivMsgTab[i].CmdPubName, command ) )
		{
			valid = true;
			break;
		}
	}

	if( valid == false )
	{
		Net->Send( "%s O %s :%s is not a known command.\n", nDst.c_str(), nSrc.c_str(), command.c_str() );
		return CMD_ERROR;
	}

	fileName = Net->GetConfig( "HELPDIR" );
	fileName.append( "/" );
	fileName.append( command );

	helpFile.open( fileName.c_str() );

	if( helpFile.fail() )
	{
		Net->Send( "%s O %s :No help available for command %s.\n", nDst.c_str(), nSrc.c_str(), 
			command.c_str() );
		return CMD_ERROR;
	}

	while( !helpFile.eof() )
	{
		getline( helpFile, strLine );

		if( strLine.length() == 0 )
			strLine = " " + strLine;

		while( (nPos = strLine.find( "%N" )) && nPos != string::npos )
			strLine.replace( nPos, 2, bot->GetNick() );

		while( (sPos = strLine.find( "%S" )) && sPos != string::npos )
			strLine.replace( sPos, 2, Net->GetConfig( "SERVNAME" ) );

		while( (lPos = strLine.find( "%L" )) && lPos != string::npos )
			strLine.replace( lPos, 2, intToStr( PrivMsgTab[i].CmdLevel ) );

		Net->Send( "%s O %s :%s\n", nDst.c_str(), nSrc.c_str(), strLine.c_str() );
	}

	helpFile.close();

	Report( CMD_HELP, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}

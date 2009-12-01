#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "stealth.h"
#include "config.h"
#include "cmdmap.h"
#include "token.h"


Config::Config() :
	c_filename( "stealth.conf" )
{
}


Config::Config( string filename ) :
	c_filename( filename )
{
}


Config::~Config()
{
	c_options.clear();
}


bool Config::LoadConfiguration()
{
	ifstream confFile;
	string strThisLine, thisParam, thisValue;
	int breakPos = -1, lineLen = 0, delimPos = -1;
	
	confFile.open( c_filename.c_str() );

	if( confFile.fail() ) 
	{
		cerr << "Error opening configuration file! ( " << c_filename << " )" << endl;
		return false;
	}
	
	
	while( !confFile.eof() )
	{
		getline( confFile, strThisLine );
		
		breakPos = strThisLine.find( "\n" );
		if( breakPos != -1 ) {
			strThisLine[breakPos] = '\0';
		}
		
		lineLen = strThisLine.length();
		if( lineLen == 0 || ( lineLen > 0 && IsComment( strThisLine ) ) )
		{
			continue;	// this line is either commented out, or blank
		}
		
		delimPos = strThisLine.find( "=" );
		if( delimPos == -1 ) // Can't find a delimiter on this line
			continue;
		
		thisParam = strThisLine.substr( 0, delimPos );
		thisValue = strThisLine.substr( delimPos + 1, lineLen );

		thisParam = RemoveWhitespace( thisParam );
		thisParam = RemoveBreaks( thisParam );
		thisValue = RemoveWhitespace( thisValue );
		thisValue = RemoveBreaks( thisValue );

		thisParam = strUpper( thisParam );

		c_options[thisParam] = thisValue;

	}

	confFile.close();

	return true;
}


bool Config::SaveConfiguration()
{
	return 1;
}


string Config::RemoveBreaks( string strLine )
{
	int breakPos = strLine.find( "\n" );

	if( breakPos != -1 )
		strLine = strLine.substr( 0, breakPos );

	return strLine;
}


string Config::RemoveWhitespace( string strLine )
{
	int foundDelimiter = 0, delimiterPos = -1;
	unsigned int c = 0;
	
	for( c = 0; c < strLine.length() && strLine[c] == ' '; c++ ) 
		strLine = strLine.substr( c + 1, strLine.length() );

	for( c = strLine.length() - 1; c >= 0 && strLine[c] == ' '; c-- ) 
		strLine = strLine.substr( 0, c );

	delimiterPos = strLine.find( "=" );

	for( c = 0; c < strLine.length() && foundDelimiter == 0; c++ ) 
	{
		if( strLine[c] == '=' ) 
		{
			foundDelimiter = 1;
			break;
		}
		else if( delimiterPos > -1 && strLine[c] == ' ' )
		{
			strLine = strLine.substr( 0, c ) + strLine.substr( c + 1, strLine.length() );
		}
	}
	
	return strLine;
}


string Config::GetParam( string paramName )
{
	return c_options[ paramName ];
}


void Config::SetParam( string paramName, string newValue )
{
	c_options[ paramName ] = newValue;
}


bool Config::IsComment( string line )
{
	unsigned int len = line.length();

	if( len == 0 )
		return false;

	if( line[0] == ';' )
		return true;
	else if( len > 1 && line.substr( 0, 2 ) == "//" )
		return true;
	else
		return false;
}


bool Config::LoadUsers()
{
	string strThisLine, usrNick, usrPasswd, usrMasks, fileName = c_options[ "DATADIR" ];
	int usrLevel = 0;
	bool usrProtect = false, usrSuspend = false;
	time_t usrLastSeen;
	ifstream uFile;
	Token thisLine;

	fileName.append( "/users.dat" );

	uFile.open( fileName.c_str() );

	if( uFile.fail() )
	{
		cerr << "Could not open user file from " << fileName << endl;
		return false;
	}

	while( !uFile.eof() )
	{
		getline( uFile, strThisLine );
		thisLine.Tokenize( strThisLine );

		if( thisLine.numTokens() < 6 || IsComment( strThisLine ) )
			continue;

		usrNick = thisLine[ 0 ];
		usrLevel = atoi( thisLine[ 1 ].c_str() );
		usrProtect = atoi( thisLine[ 2 ].c_str() );
		usrSuspend = atoi( thisLine[ 3 ].c_str() );
		usrLastSeen = atol( thisLine[ 4 ].c_str() );
		usrPasswd = thisLine[ 5 ];
		usrMasks = thisLine.Assemble( 7 );
		
		Net->AddAccessEntry( usrNick, usrLevel, usrProtect, usrSuspend, usrLastSeen, 
			usrPasswd, usrMasks );
	}

	uFile.close();
	
	return true;
}


bool Config::LoadJupes()
{
	ifstream jFile;
	time_t srvLastMod, srvDuration, srvExpire;
	string strLine, srvName, srvAdmin, srvDesc, fileName = c_options[ "DATADIR" ];
	bool srvActive;
	Token thisLine;
	Jupe *jupe;

	fileName.append( "/jupes.dat" );

	jFile.open( fileName.c_str() );

	if( jFile.fail() )
	{
		cerr << "Could not open jupes file from " << fileName << endl;
		return false;
	}
	
	while( !jFile.eof() )
	{
		getline( jFile, strLine );
		thisLine.Tokenize( strLine );

		if( thisLine.numTokens() < 5 || IsComment( strLine ) )
			continue;

		srvName = thisLine[ 0 ];
		srvExpire = atol( thisLine[ 1 ].c_str() );
		srvLastMod = atol( thisLine[ 2 ].c_str() );
		srvActive = atoi( thisLine[ 3 ].c_str() );
		srvAdmin = thisLine[ 4 ];
		srvDesc = thisLine.Assemble( 6 );

		if( srvExpire <= time( NULL ) )
			continue;

		srvDuration = srvExpire - time( NULL );
		jupe = Net->AddJupe( srvName, srvDuration, srvLastMod, srvDesc, srvAdmin, srvActive );
	}

	jFile.close();

	return true;
}


bool Config::LoadClones()
{
	ifstream cFile;
	string strLine, fileName = c_options[ "DATADIR" ];
	string cloneNick, cloneIdent, cloneHost, cloneModes, cloneDesc, cloneNum, cloneIP;
	Token thisLine;

	fileName.append( "/clones.dat" );

	cFile.open( fileName.c_str() );

	if( cFile.fail() )
	{
		cerr << "Could not open clone file from " << fileName << endl;
		return false;
	}

	while( !cFile.eof() )
	{
		getline( cFile, strLine );
		thisLine.Tokenize( strLine );

		if( thisLine.numTokens() < 5 || IsComment( strLine ) )
			continue;

		cloneNick = thisLine[0];
		cloneIdent = thisLine[1];
		cloneHost = thisLine[2];
		cloneModes = thisLine[3];
		cloneDesc = thisLine.Assemble( 5 );
		cloneNum = Net->GetNextNumeric( Net->GetConfig( "SERVNUM" ) );
		cloneIP = "AAAAAA";

		if( Net->FindClientByNick( cloneNick ) == NULL )
		{
			Net->AddClient( Net->GetConfig( "SERVNUM" ), cloneNick, 1, time( NULL ),
				cloneIdent, cloneHost, cloneModes, cloneIP, cloneNum, cloneDesc );
		}
	}

	cFile.close();

	return true;
}


bool Config::LoadChans()
{
	ifstream cFile;
	time_t chanTS = time( NULL );
	string strLine, fileName = c_options[ "DATADIR" ];
	string chanName, chanModes, botNumeric;
	ChannelClient *chanClient;
	Channel *chan;
	Token thisLine;

	botNumeric = Net->GetConfig( "SERVNUM" );
	botNumeric.append( "AAA" );

	fileName.append( "/chans.dat" );

	cFile.open( fileName.c_str() );

	if( cFile.fail() )
	{
		cerr << "Could not open channel file from " << fileName << endl;
		return false;
	}

	while( !cFile.eof() )
	{
		getline( cFile, strLine );
		thisLine.Tokenize( strLine );

		if( thisLine.numTokens() < 2 || IsComment( strLine ) )
			continue;

		chanName = thisLine[0];
		chanModes = thisLine[1];

		chan = Net->FindChannelByName( chanName );

		if( chan == NULL )
		{
			Net->AddChannel( chanName, chanTS, chanModes );
			chan = Net->FindChannelByName( chanName );
		}
		else
		{
			chan->SetModes( chanModes );
			chan->AddUser( botNumeric, "+o" );
			chanClient = chan->FindUser( botNumeric );
			chanClient->SetOp();
		}
	}
	
	return true;
}


bool Config::LoadExceptions()
{
	return true;
}


bool Config::LoadCommands()
{
	ifstream cFile;
	string fileName = c_options[ "DATADIR" ], cmdName, cmdPubName, strLine;
	int cmd, cmdLevel;
	bool cmdEnabled;
	Token thisLine;

	fileName.append( "/commands.dat" );

	cFile.open( fileName.c_str() );

	if( cFile.fail() )
	{
		cerr << "Could not open commands file from " << fileName << endl;
		return false;
	}

	while( !cFile.eof() )
	{
		getline( cFile, strLine );
		thisLine.Tokenize( strLine );

		if( thisLine.numTokens() != 4 || IsComment( strLine ) )
			continue;

		cmdName = strUpper( thisLine[0] );
		cmdPubName = strUpper( thisLine[1] );
		cmdLevel = atoi( thisLine[2].c_str() );
		cmdEnabled = atoi( thisLine[3].c_str() );

		for( cmd = 0; PrivMsgTab[cmd].CmdName != "-"; cmd++ )
		{
			if( strCmp( PrivMsgTab[cmd].CmdName, cmdName ) )
			{
				PrivMsgTab[cmd].CmdPubName = cmdPubName;
				PrivMsgTab[cmd].CmdLevel = cmdLevel;
				PrivMsgTab[cmd].CmdEnabled = cmdEnabled;
				break;
			}
		}
	}

	cFile.close();
		
	return true;
}


bool Config::SaveUsers()
{
	AccessMapType::iterator iter = Net->AccessIter();
	string fileName = c_options[ "DATADIR" ];
	ofstream uFile;
	
	fileName.append("/users.dat");
	
	uFile.open( fileName.c_str() );
	
	if( uFile.fail() )
	{
		cerr << "Could not open user file from " << fileName << endl;
		return 1;
	}

	uFile << "//" << endl;
	uFile << "// The format of this file is as follows:" << endl;
	uFile << "// nick level protected suspended lastseen password hostmasks" << endl;
	uFile << "//" << endl;
	
	for( iter = Net->AccessIter(); iter != Net->AccessTail(); iter++ )
	{
		uFile << iter->second->GetNick() << " "
		      << iter->second->GetLevel() << " "
				<< iter->second->IsProtected() << " "
				<< iter->second->IsSuspended() << " "
				<< iter->second->GetLastTS() << " "
				<< iter->second->GetPass() << " "
				<< iter->second->GetHostList() 
				<< endl;
	}
	
	uFile.close();
	
	return 0;
}


bool Config::SaveJupes()
{
	JupeMapType::iterator iter = Net->JupeIter();
	string fileName = c_options[ "DATADIR" ], jupeAdmin;
	ofstream jFile;

	fileName.append( "/jupes.dat" );

	jFile.open( fileName.c_str() );

	if( jFile.fail() )
	{
		Debug( PROG_ERR, "SaveJupes", "Could not open jupe file from %s", fileName.c_str() );
		return false;
	}

	jFile << "//" << endl;
	jFile << "// The format of this file is as follows:" << endl;
	jFile << "// servname expireTS modifyTS active admin description" << endl;
	jFile << "//" << endl;

	for( iter = Net->JupeIter(); iter != Net->JupeTail(); iter++ )
	{
		if( iter->second == NULL )
			continue;

		jupeAdmin = iter->second->GetAdmin();
		if( jupeAdmin.length() == 0 )
			jupeAdmin = "(Unknown)";

		jFile << iter->second->GetName() << " "
				<< iter->second->GetExpireTS() << " "
		      << iter->second->GetStartTS() << " "
				<< iter->second->IsActive() << " "
		      << jupeAdmin << " "
		      << iter->second->GetReason() << " "
		      << endl;
	}

	jFile.close();
	
	return true;
}


bool Config::SaveClones()
{
	ClientMapType::iterator iter = Net->ClientIter();
	string fileName = c_options[ "DATADIR" ];
	ofstream cFile;

	fileName.append( "/clones.dat" );

	cFile.open( fileName.c_str() );

	if( cFile.fail() )
	{
		Debug( PROG_ERR, "SaveClones", "Could not open clone file from %s", fileName.c_str() );
		return false;
	}

	cFile << "//" << endl;
	cFile << "// The format of this file is as follows:" << endl;
	cFile << "// nick ident host modes description" << endl;
	cFile << "//" << endl;

	for( iter = Net->ClientIter(); iter != Net->ClientTail(); iter++ )
	{
		if( strSCmp( getServNum( iter->second->GetNumeric() ), Net->GetConfig( "SERVNUM" ) )
				&& ( iter->second->GetNumeric().substr( 2, 3 ) != "AAA" ) )
		{
			cFile << iter->second->GetNick() << " "
					<< iter->second->GetUserName() << " "
					<< iter->second->GetHostName() << " "
					<< iter->second->GetModeString() << " "
					<< iter->second->GetRealName()
					<< endl;
		}
	}

	cFile.close();

	return true;
}


bool Config::SaveChans()
{
	ChannelMapType::iterator iter = Net->ChannelIter();
	string fileName = c_options[ "DATADIR" ], userNum;
	ofstream cFile;

	fileName.append( "/chans.dat" );

	cFile.open( fileName.c_str() );

	if( cFile.fail() )
	{
		Debug( PROG_ERR, "SaveChans", "Could not open channel file from %s", fileName.c_str() );
		return false;
	}

	userNum = Net->GetConfig( "SERVNUM" );
	userNum.append( "AAA" );

	cFile << "//" << endl;
	cFile << "// The format of this file is as follows:" << endl;
	cFile << "// channame chanmodes" << endl;
	cFile << "//" << endl;

	for( iter = Net->ChannelIter(); iter != Net->ChannelTail(); iter++ )
	{
		if( iter->second->FindUser( userNum ) )
		{
			cFile << iter->second->GetName() << " "
			      << iter->second->GetModeString()
			      << endl;
		}
	}

	cFile.close();

	return true;
}


bool Config::SaveCommands()
{
	int c = 0;
	string fileName = c_options[ "DATADIR" ];
	ofstream cFile;

	fileName.append( "/commands.dat" );

	cFile.open( fileName.c_str() );

	if( cFile.fail() )
	{
		Debug( PROG_ERR, "SaveCommands", "Could not open commands file from %s",
				fileName.c_str() );
		return false;
	}

	cFile << "//" << endl
		<< "// The format of this file is as follows:" << endl
		<< "// internal_name public_name level disabled" << endl
		<< "//" << endl
		<< "// DO NOT modify the internal name!" << endl
		<< "//" << endl;

	for( c = 0; PrivMsgTab[c].CmdName != "-"; c++ )
	{
		cFile << PrivMsgTab[c].CmdName << " "
			<< PrivMsgTab[c].CmdPubName << " "
			<< PrivMsgTab[c].CmdLevel << " "
			<< PrivMsgTab[c].CmdEnabled << endl;
	}

	cFile.close();

	return true;
}

#include <sstream>
#include "stealth.h"
#include "cmdmap.h"

using namespace std;

cmdStatusType cmd_MODE ( Numeric nSrc, Numeric nDst, Token tokens )
{
	Client *user;
	Channel *chan = Net->FindChannelByName( tokens[4] );
	ChannelClient *chanUsr;
	string modeString = tokens[5], modes, params;
	unsigned int i = 0, curParam = 6, modeCount = 0;
	int paramCount = 0;
	bool add = true, rem = false;
	stringstream modeBuf, paramBuf;

	if( chan == NULL )
	{
		Net->Send( "%s O %s :Channel %s does not exist.\n", nDst.c_str(), nSrc.c_str(),
			tokens[4].c_str() );
		return CMD_ERROR;
	}

	for( i = 0; i < modeString.length(); i++ )
	{
		if( modeString[i] == 'b' || modeString[i] == 'k' || modeString[i] == 'l' ||
				modeString[i] == 'o' || modeString[i] == 'v' )
			paramCount++;
	}

	if( paramCount > tokens.numTokens() - 6 )
	{
		Net->Send( "%s O %s :Too few parameters.\n", nDst.c_str(), nSrc.c_str() );
		return CMD_ERROR;
	}
	
	for( i = 0; i < modeString.length(); i++ )
	{
		if( modeString[i] == '+' )
		{
			modeBuf << "+";
			add = true;
			rem = false;
			continue;
		}
		if( modeString[i] == '-' )
		{
			modeBuf << "-";
			add = false;
			rem = true;
			continue;
		}
		
		if( add )
		{
			switch( modeString[i] )
			{
				case 'b':
					chan->AddBan( tokens[curParam] );
					modeBuf << "b";
					paramBuf << " " << tokens[curParam];
					curParam++;
					break;

				case 'o':
					user = Net->FindClientByNick( tokens[curParam] );

					if( user == NULL )
					{
						Net->Send( "%s O %s :User %s does not exist.\n", nDst.c_str(), nSrc.c_str(),
							tokens[curParam].c_str() );
						curParam++;
						continue;
					}

					chanUsr = chan->FindUser( user->GetNumeric() );

					if( chanUsr == NULL )
					{
						Net->Send( "%s O %s :User %s is not on %s.\n", nDst.c_str(), nSrc.c_str(),
							user->GetNick().c_str(), chan->GetName().c_str() );
						curParam++;
						continue;
					}

					chanUsr->SetOp();
					modeBuf << "o";
					paramBuf << " " << user->GetNumeric();
					modeCount++;
					curParam++;

					break;

				case 'v':
					user = Net->FindClientByNick( tokens[curParam] );

					if( user == NULL )
					{
						Net->Send( "%s O %s :User %s does not exist.\n", nDst.c_str(), nSrc.c_str(),
							tokens[curParam].c_str() );
						curParam++;
						continue;
					}

					chanUsr = chan->FindUser( user->GetNumeric() );

					if( chanUsr == NULL )
					{
						Net->Send( "%s O %s :User %s is not on %s.\n", nDst.c_str(), nSrc.c_str(),
							user->GetNick().c_str(), chan->GetName().c_str() );
						curParam++;
						continue;
					}

					chanUsr->SetVoice();
					modeBuf << "v";
					paramBuf << " " << user->GetNumeric();
					modeCount++;
					curParam++;

					break;

				case 'k':
					chan->AddMode( channelFlags::CHAN_KEY );
					chan->SetKey( tokens[curParam] );
					modeBuf << "k";
					paramBuf << " " << tokens[curParam];
					modeCount++;
					curParam++;

					break;

				case 'l':
					chan->AddMode( channelFlags::CHAN_LIMIT );
					chan->SetLimit( atoi( tokens[curParam].c_str() ) );
					modeBuf << "l";
					paramBuf << " " << tokens[curParam];
					modeCount++;
					curParam++;
					
					break;

				case 'i':
					chan->AddMode( channelFlags::CHAN_INVITE );
					modeBuf << "i";
					modeCount++;
					break;

				case 'm':
					chan->AddMode( channelFlags::CHAN_MODERATED );
					modeBuf << "m";
					modeCount++;
					break;

				case 'n':
					chan->AddMode( channelFlags::CHAN_NOEXMSGS );
					modeBuf << "n";
					modeCount++;
					break;

				case 'p':
					if( !chan->CheckMode( channelFlags::CHAN_SECRET ) )
					{
						chan->AddMode( channelFlags::CHAN_PRIVATE );
						modeBuf << "p";
						modeCount++;
					}
					break;

				case 's':
					if( !chan->CheckMode( channelFlags::CHAN_PRIVATE ) )
					{
						chan->AddMode( channelFlags::CHAN_SECRET );
						modeBuf << "s";
						modeCount++;
					}
					break;

				case 't':
					chan->AddMode( channelFlags::CHAN_TOPICOP );
					modeBuf << "t";
					modeCount++;
					break;
			}
		}
		else if( rem )
		{
			switch( modeString[i] )
			{
				case 'b':
					chan->RemBan( tokens[curParam] );
					modeBuf << "b";
					paramBuf << " " << tokens[curParam];
					curParam++;
					break;

				case 'o':
					user = Net->FindClientByNick( tokens[curParam] );

					if( user == NULL )
					{
						Net->Send( "%s O %s :User %s does not exist.\n", nDst.c_str(), nSrc.c_str(),
							tokens[curParam].c_str() );
						curParam++;
						continue;
					}

					chanUsr = chan->FindUser( user->GetNumeric() );

					if( chanUsr == NULL )
					{
						Net->Send( "%s O %s :User %s is not on %s.\n", nDst.c_str(), nSrc.c_str(),
							user->GetNick().c_str(), chan->GetName().c_str() );
						curParam++;
						continue;
					}

					chanUsr->RemOp();
					modeBuf << "o";
					paramBuf << " " << user->GetNumeric();
					modeCount++;
					curParam++;

					break;

				case 'v':
					user = Net->FindClientByNick( tokens[curParam] );

					if( user == NULL )
					{
						Net->Send( "%s O %s :User %s does not exist.\n", nDst.c_str(), nSrc.c_str(),
							tokens[curParam].c_str() );
						curParam++;
						continue;
					}

					chanUsr = chan->FindUser( user->GetNumeric() );

					if( chanUsr == NULL )
					{
						Net->Send( "%s O %s :User %s is not on %s.\n", nDst.c_str(), nSrc.c_str(),
							user->GetNick().c_str(), chan->GetName().c_str() );
						curParam++;
						continue;
					}

					chanUsr->RemVoice();
					modeBuf << "v";
					paramBuf << " " << user->GetNumeric();
					modeCount++;
					curParam++;

					break;

				case 'k':
					chan->RemMode( channelFlags::CHAN_KEY );
					modeBuf << "k";
					paramBuf << " *";
					modeCount++;
					break;

				case 'l':
					chan->RemMode( channelFlags::CHAN_LIMIT );
					modeBuf << "l";
					modeCount++;
					break;

				case 'i':
					chan->RemMode( channelFlags::CHAN_INVITE );
					modeBuf << "i";
					modeCount++;
					break;

				case 'm':
					chan->RemMode( channelFlags::CHAN_MODERATED );
					modeBuf << "m";
					modeCount++;
					break;

				case 'n':
					chan->RemMode( channelFlags::CHAN_NOEXMSGS );
					modeBuf << "n";
					modeCount++;
					break;

				case 'p':
					chan->RemMode( channelFlags::CHAN_PRIVATE );
					modeBuf << "p";
					modeCount++;
					break;

				case 's':
					chan->RemMode( channelFlags::CHAN_SECRET );
					modeBuf << "s";
					modeCount++;
					break;

				case 't':
					chan->RemMode( channelFlags::CHAN_TOPICOP );
					modeBuf << "t";
					modeCount++;
					break;
			}
		}


		if( modeCount == 6 )
		{
			modes = modeBuf.str();
			params = paramBuf.str();
			if( params[0] == ' ' )
				params = params.substr( 1, params.length() );
			
			Net->Send( "%s M %s %s %s\n", Net->GetConfig( "SERVNUM" ), chan->GetName().c_str(),
				modes.c_str(), params.c_str() );
			modeBuf.clear();
			paramBuf.clear();
			modes.erase();
			params.erase();
			modeCount = 0;
		}
	}

	if( modeCount > 0 )
	{
		modes = modeBuf.str();
		params = paramBuf.str();
		if( params[0] == ' ' )
			params = params.substr( 1, params.length() );
		
		Net->Send( "%s M %s %s %s\n", Net->GetConfig( "SERVNUM" ), chan->GetName().c_str(),
			modes.c_str(), params.c_str() );
		modeBuf.clear();
		paramBuf.clear();
		modes.erase();
		params.erase();
		modeCount = 0;
	}


	Report( CMD_MODE, nSrc, 0, tokens.Assemble( 5 ).c_str() );

	return CMD_SUCCESS;
}


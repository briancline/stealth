#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_M ( Numeric nSrc, Numeric nDst, Token params )
{
	bool isChannel = false, add = false, rem = false;
	long unsigned int i = 0, curParam = 4;
	string modes, num;
	ChannelClient *chanUser;
	Channel *chan;
	Client *user;

	modes = params[3];
	if( modes[0] == ':' )
		modes = modes.substr( 1, modes.length() );

	if( params[2][0] == '#' )
		isChannel = true;
	else
		isChannel = false;

	if( isChannel )
	{
		chan = Net->FindChannelByName( params[2] );
		assert( chan != NULL );
		
		for( i = 0; i < modes.length(); i++ )
		{
			if( modes[i] == '+' )
			{
				add = true;
				rem = false;
				continue;
			}
			else if( modes[i] == '-' )
			{
				add = false;
				rem = true;
				continue;
			}

			if( add )
			{
				switch( modes[i] )
				{
					case 'l':
						chan->SetLimit( atoi( params[curParam].c_str() ) );
						chan->AddMode( modes[i] );
						curParam++;
						break;


					case 'k':
						chan->SetKey( params[curParam] );
						chan->AddMode( modes[i] );
						curParam++;
						break;


					case 'o':
						num = params[curParam].substr( 0, 5 );
						chanUser = chan->FindUser( num );
						
						assert( chanUser != NULL );
						
						chanUser->SetOp();
						curParam++;
						break;


					case 'v':
						num = params[curParam].substr( 0, 5 );
						chanUser = chan->FindUser( num );

						assert( chanUser != NULL );

						chanUser->SetVoice();
						curParam++;
						break;


					case 'b':
						chan->AddBan( params[curParam] );

						curParam++;
						break;


					default:
						chan->AddMode( modes[i] );

				}
			}
			else if( rem )
			{
				switch( modes[i] )
				{
					case 'b':
						chan->RemBan( params[curParam] );
						curParam++;
						break;


					case 'o':
						num = params[curParam].substr( 0, 5 );
						chanUser = chan->FindUser( num );

						assert( chanUser != NULL );

						chanUser->RemOp();
						curParam++;
						break;


					case 'v':
						num = params[curParam].substr( 0, 5 );
						chanUser = chan->FindUser( num );

						assert( chanUser != NULL );

						chanUser->RemVoice();
						curParam++;
						break;
					

					default:
						chan->RemMode( modes[i] );

				}
			}
		}

		Report( SRV_CHMODE, nSrc, 0, params.Assemble( 4 ).c_str(), chan->GetName().c_str() );
	}
	else
	{
		user = Net->FindClientByNum( params[0] );
		assert( user != NULL );
		
		for( i = 0; i < modes.length(); i++ )
		{
			if( modes[i] == '+' )
			{
				add = true;
				rem = false;
				continue;
			}
			else if( modes[i] == '-' )
			{
				add = false;
				rem = true;
				continue;
			}
			
			if( add )
			{
				user->AddMode( modes[i] );
			}
			else if( rem )
			{
				user->RemMode( modes[i] );

				if( modes[i] == 'o' && user->IsAuthed() )
				{
					user->ClearAccess();
					Net->Send( "%sAAA O %s :You have been deauthenticated.\n", Net->GetConfig("SERVNUM"), params[0].c_str() );
				}
			}
		}

		Report( SRV_UMODE, nSrc, 0, modes.c_str(), user->GetNick().c_str() );
	}

	return MSG_SUCCESS;
}


#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include "stealth.h"
#include "network.h"
#include "signals.h"

using namespace std;


int main( int argc, char *argv[] ) 
{
	Net = new Network;

	if( argc > 1 )
	{
		runningConfig = argv[1];
		Net->LoadConfig( runningConfig );
	}
	else
	{
		runningConfig = FILE_CONF;
		Net->LoadConfig();
	}

	/*
	if( fork() )
		exit( 0 );
	*/

	writePid( argv[0] );
	
	RegisterSignals();
	
	connecting = true;
	while( connecting )
	{
		if( Net->Connect() )
		{
			connecting = false;
			RegisterTimers();
			Net->Register();
			Net->Main();

			if( connecting )
			{
				timerVector.clear();
				Net->LoadConfig( runningConfig );
			}

			if( dying )
				break;
		}
		else 
		{
			connecting = false;
			Debug( PROG_ERR, "main", "Connect failed!" );
			return EXIT_FAILURE;
		}
	}
	
	return EXIT_SUCCESS;
}

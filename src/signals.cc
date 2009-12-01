#include <csignal>
#include "signals.h"
#include "stealth.h"


void RegisterSignals()
{
	signal( SIGTERM, SignalHandler );
	signal( SIGINT, SignalHandler );
	signal( SIGHUP, SignalHandler );
//	signal( SIGSEGV, SignalHandler );
}


void SignalHandler( int sigType )
{
	switch( sigType )
	{
		case SIGSEGV:
			SigSegfault();
			break;

		case SIGINT:
			SigInterrupt();
			break;

		case SIGTERM:
			SigTerminate();
			break;

		case SIGHUP:
			SigHangup();
			break;

		default:
			Debug( PROG_ERR, "SignalHandler", "Received unhandled signal %d", sigType );
			break;
	}
}


void SigInterrupt()
{
	// SIGINT mostly ignored
	Net->SaveData();
}


void SigSegfault()
{
	string servNum = Net->GetConfig( "SERVNUM" );

	Net->Send( "%s WA :Got SIGSEGV :(\n", servNum.c_str() );
	Net->Send( "%s SQ %s 0 :Caught SIGSEGV, quitting\n", servNum.c_str(), Net->GetConfig( "SERVNAME" ) );

	exit( 1 );
}


void SigTerminate()
{
	// SIGTERM mostly ignored
	Net->SaveData();
}


void SigHangup()
{
	string servNum = Net->GetConfig( "SERVNUM" );

	Net->Send( "%s WA :Caught SIGHUP, restarting\n", servNum.c_str() );
	Net->Send( "%s SQ %s 0 :Restarting\n", servNum.c_str(), Net->GetConfig( "SERVNAME" ) );

	connecting = true;
}

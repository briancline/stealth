/*
 * socket.h - Socket class
 * Originally from the GPL-licensed pgworld project
 * Written by Jonathan C. O'Byrne ( Chase@IRC )
 * http://sourceforge.net/projects/pgworld/
 *
 * Modifications for stealth by Brian A. Cline ( Trask@IRC )
 *
 */

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include "stealth.h"
#include "socket.h"

#define __SOCKET_CC__ "$Id: socket.cc,v 1.35 2002/04/14 06:48:37 bcline Exp $"

using namespace std;


Socket::Socket() 
{ 
	bytesRecv = 0;
	bytesSent = 0;
	newfd = 0; 
	Create( TCP ); // default type is TCP
}


Socket::Socket( char nType )
{	
	bytesRecv = 0;
	bytesSent = 0;
	newfd = 0;
	Create( nType ); 
}


Socket::Socket( char nType, int nPort )
{
	bytesRecv = 0;
	bytesSent = 0;
	newfd = 0;
	Create( nPort, nType );
}


Socket::~Socket() 
{ }


bool Socket::Create() 
{
	sockfd = socket( AF_INET, SOCK_STREAM, 0 );

	if( sockfd == -1 )
		return false;

	return true;
};


bool Socket::Create( char nType ) 
{
	if( nType == TCP )
		return Create();
	if( nType == UDP )
	{
		sockfd = socket( AF_INET, SOCK_DGRAM, 0 );

		if( sockfd == -1 )
			return false;

		return true;
	}
	return false;
}


bool Socket::Connect( int nPort, char *Hostname ) 
{
	hostent *Host = new hostent;
	
	if( ( Host = gethostbyname( Hostname ) ) == NULL )
	{
		herror( "gethostbyname" );
		return false;
	}
	
	INetAddress.sin_family = AF_INET;
	INetAddress.sin_port = htons( nPort );
	INetAddress.sin_addr = *( ( struct in_addr * )Host->h_addr );
	bzero( &( INetAddress.sin_zero ), 8 );

	if( connect( sockfd, ( struct sockaddr * )&INetAddress, sizeof( struct sockaddr ) ) == -1 )
	{
		perror( "connect" );
		return false;
	}

//	fcntl( sockfd, F_SETFL, O_NONBLOCK );

	return true;
}


bool Socket::Connect( int nPort, string Hostname )
{
	char host[512];
	sprintf( host, "%s", Hostname.c_str() );
	return Connect( nPort, host );
}


bool Socket::Connect( string Hostname, int nPort )
{
	char host[512];
	sprintf( host, "%s", Hostname.c_str() );
	return Connect( nPort, host );
}


int Socket::Send( const char *Message, int nLength ) 
{
	int Bytes_Sent;
	
	if( nLength == -2 )
		nLength = strlen( Message );

#ifdef SOCKDEBUG
	AppendLog( "socket.log", "[SEND] %s", Message );
#endif

	if( newfd==0 )
		Bytes_Sent = send( sockfd, Message, nLength, 0 );
	else 
		Bytes_Sent = send( newfd, Message, nLength, 0 );

	bytesSent += nLength;
	
	return Bytes_Sent;
}


int Socket::Send( string Message )
{
	return Send( Message.c_str() );
}


int Socket::Sendf( const char* Message, ... )
{
	va_list arg;
	char *buf = new char[4096];
	
	va_start( arg, Message );
	vsprintf( buf, Message, arg );
	va_end( arg );
	
	return ( Send( buf ) );
}


int Socket::Recv( char *Buffer, int nLength ) 
{
	char ch;
	int i = 0, fd = 0;

	fd = ( newfd == 0 ) ? sockfd : newfd;

	do
	{
		if( read( fd, &ch, 1 ) < 1 )
			return 1;

		if( ch != '\0' && ch != '\n' && ch != '\r' )
			Buffer[i++] = ch;

	} while( ch != '\n' );

	Buffer[i] = '\0';
	bytesRecv += i;

#ifdef SOCKDEBUG
	AppendLog( "socket.log", "[RECV] %s", Buffer );
#endif

	return 1;
}


int Socket::Recv( char *Buffer )
{
	return Recv( Buffer, 1024 );
}



int Socket::Recv( string Buffer, int nLength )
{
	char cBuffer[nLength];
	int bytes;

	bytes = Recv( cBuffer, nLength );
	strCopy( Buffer, cBuffer );

	return bytes;
}


int Socket::Recv( string Buffer )
{
	char cBuffer[1024];
	int bytes;

	bytes = Recv( cBuffer, 1024 );
	strCopy( Buffer, cBuffer );

	return bytes;
}


bool Socket::Close() 
{
	int Result;

	if( newfd == 0 ) 
		Result = close( sockfd );
	else 
		Result = close( newfd );

	if( Result == -1 ) 
		return false;

	return true;
}


bool Socket::InboundClose() 
{
	int Result;

	Result = close( newfd );

	if( Result == -1 )
		return false;

	return true;
}


bool Socket::Create( int nPort ) 
{
	sockfd = socket( AF_INET, SOCK_STREAM, 0 );  
	
	if( sockfd==-1 )
		return false;
	
	INetAddress.sin_family = AF_INET;
	INetAddress.sin_port = htons( nPort );
	INetAddress.sin_addr.s_addr = INADDR_ANY;
	bzero( &( INetAddress.sin_zero ), 8 );
	
	if( bind( sockfd, ( struct sockaddr * )&INetAddress, sizeof( struct sockaddr ) ) == -1 )
	{
		perror( "bind" );
		return false;
	}

	return true;
}


bool Socket::Create( int nPort, char nType ) 
{
	if( nType == TCP ) 
	{
		sockfd = socket( AF_INET, SOCK_STREAM, 0 );  
		if( sockfd==-1 )
			return false;
	}

	if( nType == UDP ) 
	{
		sockfd = socket( AF_INET, SOCK_DGRAM, 0 );
		if( sockfd==-1 )
			return false;
	}

	INetAddress.sin_family = AF_INET;
	INetAddress.sin_port = htons( nPort );
	INetAddress.sin_addr.s_addr = INADDR_ANY;
	bzero( &( INetAddress.sin_zero ), 8 );
	
	if( bind( sockfd, ( struct sockaddr * )&INetAddress, sizeof( struct sockaddr ) ) == -1 )
	{
		perror( "bind" );
		return false;
	}

	return true;
}


bool Socket::Create( char nType, int nPort ) 
{
	return Create( nPort, nType );
}


bool Socket::Listen() 
{
	if( listen( sockfd, 5 ) == -1 )
	{
		perror( "Listen" );
		return false;
	}
	
	return true;
}


bool Socket::Accept() 
{
	size_t sin_size = sizeof( struct sockaddr_in );
	sockaddr_in RemoteAddress;
	if( ( newfd = accept( sockfd, ( struct sockaddr * )&RemoteAddress, &sin_size ) ) == -1 )
	{
			perror( "accept" );
			return false;
	}
	return true;
}


unsigned long int Socket::GetBytesRecv()
{
	return bytesRecv;
}


unsigned long int Socket::GetBytesSent()
{
	return bytesSent;
}


int Socket::GetFD()
{
	return sockfd;
}

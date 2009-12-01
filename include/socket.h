/*
 * socket.h - Socket class
 * Originally from the GPL-licensed pgworld project
 * Written by Jonathan C. O'Byrne ( Chase@IRC )
 * http://sourceforge.net/projects/pgworld/
 *
 * Modifications for stealth by Brian A. Cline ( Trask@IRC )
 *
 */

#ifndef __SOCKET_H__
#define __SOCKET_H__ "$Id: socket.h,v 1.17 2002/03/26 06:44:47 bcline Exp $"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string>

#define TCP 'T'
#define UDP 'U'

using namespace std;


class Socket {

	public:
		Socket();
		Socket( char nType );
		Socket( char nType, int nPort );
		~Socket();

		bool Create();
		bool Create( char nType );
		bool Create( int nPort );
		bool Create( int nPort, char nType );
		bool Create( char nType, int nPort );

		bool Listen();
		bool Accept();

		bool Connect( int nPort, char *Hostname );
		bool Connect( int nPort, string Hostname );
		bool Connect( string Hostname, int nPort );

		int Send( const char* Message, int nLength = -2 );
		int Send( string Message );

		int Sendf( const char* Message, ... );
		int Sendf( string Message, ... );

		int Recv( char *Buffer, int nLength );
		int Recv( char *Buffer );
		int Recv( string Buffer );
		int Recv( string Buffer, int nLength );

		bool Close();
		bool InboundClose();

		unsigned long int GetBytesRecv();
		unsigned long int GetBytesSent();

		int GetFD();


	private:
		int sockfd;
		int newfd;
		unsigned long int bytesRecv;
		unsigned long int bytesSent;
		sockaddr_in INetAddress;
		sockaddr_in RemoteAddress;

};


#endif // __SOCKET_H__

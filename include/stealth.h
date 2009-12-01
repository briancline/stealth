#ifndef __STEALTH_H__
#define __STEALTH_H__ "$Id: stealth.h,v 1.39 2002/04/26 04:45:34 bcline Exp $"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <ctime>
#include "network.h"
#include "types.h"
#include "timer.h"
#include "events.h"

using namespace std;

#define NICKLEN 12
#define STEALTH_URL "http://stealth.ircplanet.net"
#define FILE_CONF "stealth.conf"

typedef string Numeric;
typedef vector< Timer* > TimerVectorType;


void Debug( DebugMsgType type, char *title, char *format, ... );
void Report( ServerEventType event, string num, int n, ... );
void Report( CommandEventType event, string num, int n, ... );
bool AppendLog( string logFile, char *format, ... );
bool AppendLog( string logFile, const char *format, ... );

void strCopy( string strBuffer, char *cBuffer );
string strUpper( string strBuffer );
string strLower( string strBuffer );

// case insensitive string comparision functions
int strCmp( string s1, string s2 );
int strCmp( string s1, char *s2 );
int strCmp( char *s2, string s1 );
int strCmp( char *s1, char *s2 );

// case sensitive string comparision functions
int strSCmp( string s1, string s2 );
int strSCmp( string s1, char *s2 );
int strSCmp( char *s2, string s1 );
int strSCmp( char *s1, char *s2 );

string intToStr( int i );

string getServNum( string num );
int Base64ToInt( string Base64 );
string IntToBase64( int s, int count );
string Base64ToIP( string ipAddr );

void FormatLinks( vector<Link> &links );
void GetLinks( vector<Link> &links, Numeric srvNum, int depth );

int match( const string& s1, const string& s2 );
int cmatch( const char *mask, const char *name );
char *collapse( char *pattern );
int wwcmp( const char *old_mask, const char *new_mask );
int comp( char *str1, char *str2, int len );

void RegisterTimers();
void ExpireGlines();
void ExpireJupes();
void SaveData();
void SaveMap();
void SaveStats();
void SaveServers();

void writePid( char *binName );


extern const char IntToNum[];
extern const unsigned int NumToInt[];
extern string runningConfig;
extern bool connecting;
extern bool dying;

extern Network *Net;
extern vector< Timer* > timerVector;


#endif // __STEALTH_H__

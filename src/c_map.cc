#include <vector>
#include <string>
#include "stealth.h"
#include "cmdmap.h"
#include "network.h"
#include "types.h"

using namespace std;


cmdStatusType cmd_MAP ( Numeric nSrc, Numeric nDst, Token tokens )
{
	string servNum = Net->GetConfig( "SERVNUM" );
	vector<Link> links;
	unsigned int i = 0;

	GetLinks( links, servNum, 0 );
	FormatLinks( links );
	
	for( i = 0; i < links.size(); i++ )
	{
		Net->Send( "%s O %s :%-20s\n", nDst.c_str(), nSrc.c_str(),
			links[i].name.c_str() );
	}

	Report( CMD_MAP, nSrc, 0, 0 );

	return CMD_SUCCESS;
}


void FormatLinks( vector<Link> &links )
{
	int i = 0, j = 0, size = links.size();
	int lastindex = 0, maxdepth = 0, front = 0;
	
	if( size == 1 )
		return;
	
	for( j = 0; j < size; j++ )
	{
		if( links[j].depth > maxdepth )
			maxdepth = links[j].depth;
	}

	for( i = 1; i <= maxdepth; i++ )
	{
		for( j = 1; j < size; j++ )
			if( links[j].depth == i )
				lastindex = j;

		front = 2 * (i - 1);
		
		for( j = i; j <= lastindex; j++ )
			if( links[j].name[front-1] != '-' )
				links[j].name[front] = '|';

		links[lastindex].name[front] = '`';
	}
}


void GetLinks( vector<Link> &links, Numeric srvNum, int depth )
{
	ServerMapType::iterator serv;
	int i = 0, ind = 0, front = 0;
	Link thisServ;

	ind = links.size();

	if( depth == 0 )
	{
		thisServ.name.erase();
		thisServ.name.append( Net->GetConfig( "SERVNAME" ) );

		thisServ.depth = 0;
		depth++;
		ind++;

		links.push_back( thisServ );

		srvNum = Net->GetConfig( "SERVNUM" );
	}

	for( serv = Net->ServIter(); serv != Net->ServTail(); serv++ )
	{
		if( strSCmp( serv->second->GetUplinkNumeric(), srvNum ) &&
			!strSCmp( serv->second->GetNumeric(), srvNum ) )
		{
			thisServ.name.erase();

			for( i = 0; i < depth; i++ )
				thisServ.name.append( "  " );

			front = depth * 2;
			thisServ.name[--front] = '-';
			thisServ.name.append( serv->second->GetName() );
			thisServ.depth = depth;

			links.push_back( thisServ );
			
			GetLinks( links, serv->second->GetNumeric(), depth + 1 );
		}
	}

	if( depth > 0 )
		links[ind].depth = depth;
}


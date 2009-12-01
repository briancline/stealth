#ifndef __TYPES_H__
#define __TYPES_H__ "$Id: types.h,v 1.11 2002/02/07 04:59:08 bcline Exp $"

#include <string>
#include <vector>
#include <map>

using namespace std;


typedef unsigned short int modeType;
typedef unsigned short int flagType;

enum msgStatusType { MSG_SUCCESS, MSG_SYNTAX, MSG_ERROR };
enum cmdStatusType { CMD_SUCCESS, CMD_SYNTAX, CMD_ACCESS, CMD_ERROR };
enum DebugMsgType { CHAN_DEBUG, CHAN_INFO, PROG_INFO, PROG_ERR };

struct Link
{
	string name;
	int depth;
};

#endif // __TYPES_H__

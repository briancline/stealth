#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_PASS ( Numeric nSrc, Numeric nDst, Token params )
{
	// We don't need to do anything with the remote password
	return MSG_SUCCESS;
}


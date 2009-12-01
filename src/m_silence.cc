#include <string>
#include "stealth.h"
#include "msgmap.h"
#include "token.h"
#include "server.h"
#include "client.h"

using namespace std;


msgStatusType msg_U ( Numeric nSrc, Numeric nDst, Token params )
{
	// If, for some reason, a SILENCE msg comes through, we don't need anything with it

	Report( SRV_SILENCE, nSrc, 0, params[2].c_str() );

	return MSG_SUCCESS;
}

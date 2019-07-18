#include "stdafx.h"
#include "public/ChatClientAPI.h"
#include "CallbacksHolder.h"


void setCallbackMessageReceived(callbackMessageReceivedFunc funcPtr)
{
	CallbacksHolder::clbMessageReceive = funcPtr;
}

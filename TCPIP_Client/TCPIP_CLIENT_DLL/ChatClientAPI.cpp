#include "stdafx.h"
#include "public/ChatClientAPI.h"
#include "CallbacksHolder.h"
#include "TCPIP_CLIENT_DLL.h"

void setCallbackMessageReceived(callbackMessageReceivedFunc funcPtr)
{
	CallbacksHolder::clbMessageReceive = funcPtr;
}

void ClientMainLoop()
{
	TCPIP_Client::Instance()->ClientMainLoop();
}
void ClientSendMessage(const char* szStr)
{
	TCPIP_Client::Instance()->szHasIncomingMessage = szStr;
}

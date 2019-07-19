#include "TCPIP_CLIENT_DLL.h"
#include "public/ChatClientAPI.h"
#include "CallbacksHolder.h"

void setCallbackMessageReceived(callbackMessageReceivedFunc funcPtr)
{
	CallbacksHolder::clbMessageReceive = funcPtr;
}
void ClientMainLoop()
{
	TCPIP_Client::Instance()->ClientMainLoop();
}
void ClientSendMessage(char* szStr)
{
	TCPIP_Client::Instance()->szHasIncomingMessage = szStr;
}
void ClientTerminate()
{
	TCPIP_Client::Instance()->NeedTerminate = true;
}

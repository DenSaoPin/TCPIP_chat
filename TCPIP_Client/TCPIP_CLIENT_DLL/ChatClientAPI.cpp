#include "TCPIP_CLIENT_DLL.h"
#include "public/ChatClientAPI.h"
#include "CallbacksHolder.h"

extern "C"
{
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
		TCPIP_Client::Instance()->SendTextMessage(szStr);
	}
	void ClientTerminate()
	{
		TCPIP_Client::Instance()->Shutdown();
	}
	void SetConnectionParams(const char *name, const char *ip, const char *port)
	{
		TCPIP_Client::Instance()->Initialize(name, ip, port);
	}
}

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
		try
		{
			TCPIP_Client::Instance()->ClientMainLoop();
		}
		catch (...)
		{
			CallbacksHolder::clbMessageReceive(TCPIP_Client::Instance()->Name.c_str(), reinterpret_cast<int*>(ChatLib::eInvalid) , "Exception");
		}
	}
	void ClientSendMessage(const char* szTargetName, const int status, const void * data, const int dataLen)
	{
		std::string szName;

		if (szTargetName != nullptr)
		{
			std::string szName(szTargetName);
		}

		TCPIP_Client::Instance()->AddForSend(szName, status, data, dataLen);
	}
	void ClientTerminate()
	{
		TCPIP_Client::Instance()->Shutdown();
	}
	void SetConnectionParams(const char *name, const char *ip, const char *port)
	{
		TCPIP_Client::Instance()->Initialize(name, ip, port);
	}
	EClientStatus GetClientDllStatus()
	{
		return TCPIP_Client::Instance()->GetStatus();
	}
}

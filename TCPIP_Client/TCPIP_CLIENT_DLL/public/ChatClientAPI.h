#pragma once

#ifdef TCPIP_CLIENT_DLL_EXPORTS
#define TCPIPCLIENTDLL_EXPORT __declspec(dllexport)
#else
#define TCPIPCLIENTDLL_EXPORT __declspec(dllimport)
#endif

typedef void (*callbackMessageReceivedFunc) (const char *, const int*, const char*);

extern "C"
{
	TCPIPCLIENTDLL_EXPORT void setCallbackMessageReceived(callbackMessageReceivedFunc);
	TCPIPCLIENTDLL_EXPORT void ClientMainLoop();
	TCPIPCLIENTDLL_EXPORT void ClientSendMessage(const char *);
	TCPIPCLIENTDLL_EXPORT void ClientTerminate();
	TCPIPCLIENTDLL_EXPORT void SetConnectionParams(const char *, const char *, const char *);
	TCPIPCLIENTDLL_EXPORT bool IsWorkingState();
}
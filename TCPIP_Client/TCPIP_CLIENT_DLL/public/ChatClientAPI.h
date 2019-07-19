#pragma once

#ifdef TCPIPCLIENTDLL_EXPORTS
#define TCPIPCLIENTDLL_EXPORT __declspec(dllexport)
#else
#define TCPIPCLIENTDLL_EXPORT __declspec(dllimport)
#endif

typedef void (*callbackMessageReceivedFunc) (const char *);

extern "C"
{
	TCPIPCLIENTDLL_EXPORT void setCallbackMessageReceived(callbackMessageReceivedFunc);
	TCPIPCLIENTDLL_EXPORT void ClientMainLoop();
	TCPIPCLIENTDLL_EXPORT void ClientSendMessage(const char*);
}
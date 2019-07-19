#pragma once

#include  <public/ChatClientAPI.h>
#include <thread>

void OnRecieveMessage(const char *szMessage);

// OnSendMessage(const char *szMessage);

int main()
{
	setCallbackMessageReceived(OnRecieveMessage);

	std::thread chatThread(ClientMainLoop);

	std::string message = "test";

	while (true)
	{
		ClientSendMessage(message.c_str());
	}
}


void OnRecieveMessage(const char *szMessage)
{
	printf("%s", szMessage);
}

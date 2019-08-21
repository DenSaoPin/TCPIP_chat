#pragma once

#include  <public/ChatClientAPI.h>
#include <thread>
#include <iostream>
#include <string>
#include "ProtocolAPI/BroadcastMessage.h"


void OnRecieveMessage(const char *szName, const int* messageType, const char *szMessage);
	void OnExit();

	std::thread chatThread;

	int main()
	{
		std::string message;
		std::cin >> message;
		SetConnectionParams(message.c_str(), "127.0.0.1", "7700");
		setCallbackMessageReceived(OnRecieveMessage);

		chatThread = std::thread(ClientMainLoop);

		//TODO need close thread when console closing
		//atexit(OnExit);

		while (true)
		{
			std::getline(std::cin, message);

			if(message.empty())
				continue;

			int startIndex = message.find("for @");

			ChatLib::MessageType messageType = ChatLib::eInvalid;
			if (startIndex != std::string::npos)
			{
				startIndex += 5;
				int finishIndex = message.find("@", startIndex);
				std::string forName = message.substr(startIndex, finishIndex - startIndex);
				messageType = ChatLib::eDirectMessage;
			}
			else
			{
				messageType = ChatLib::eBroadcastMessage;
			}

			std::string szStr = message.c_str();

			ClientSendMessage(nullptr, static_cast<const int>(messageType), reinterpret_cast<const void*>(message.c_str()), szStr.size());
		}
	}

	void OnRecieveMessage(const char *szName, const int* messageType, const char *szMessage)
	{
		printf("%s: %s" "\n", szName, szMessage);
	}

	void OnExit()
	{
		ClientTerminate();

		chatThread.join();
		std::cout << "";
	}

#pragma once

#include  <public/ChatClientAPI.h>
#include <thread>
#include <iostream>
#include <string>
#include <future>

namespace Console
{
	void OnRecieveMessage(const char *szName, const int* messageType, const char *szMessage);
	void OnExit();

	std::thread chatThread;

	int main()
	{
		std::string message;
		std::cin >> message;
		SetConnectionParams(message.c_str(), "127.0.0.1", "7700");
		setCallbackMessageReceived(OnRecieveMessage);

		//chatThread = std::thread(ClientMainLoop);
		//atexit(OnExit);

		//if (!asyncThread.valid())
		//{
		//	asyncThread = std::async(std::launch::async, []
		//	{
		//		std::string s = "";
		//		if (std::getline(std::cin, s)) return s;
		//	});
		//}

		//std::string str;
		//if (asyncThread.wait_for(std::chrono::seconds(1)) == std::future_status::ready)
		//{
		//	str = asyncThread.get();

		//	asyncThread = std::async(std::launch::async, []
		//	{
		//		std::string s = "";
		//		if (std::getline(std::cin, s)) return s;
		//	});
		//}
		//else
		//{
		//	//str = " Test message \n";
		//}

		while (true)
		{
			std::getline(std::cin, message);
			ClientSendMessage(message.c_str());
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
	}
}
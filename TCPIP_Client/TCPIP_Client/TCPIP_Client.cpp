// TCPIP_Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma comment (lib, "Ws2_32.lib")

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <string>
#include <thread>
#include "../../ProtocolAPI/ProtocolAPI/Message.h"
#include <future>

#define MAX_PORT_DIGIT 5

class UIInterface
{
public:
	std::string GetName()
	{
		std::string ret;
		std::cout << "Enter your name: \n";
		std::cin >> ret;
		return  ret;
	}
	std::string GetIP()
	{
		std::cout << "Enter Server IP or press Enter for use localhost \n" << std::endl;

		//TODO Do we have best clean \n method?
		std::cin.ignore();

		//TODO check 
		std::string ret;
		std::getline(std::cin, ret);

		//TODO check
		if (ret.length() == 0)
		{
			ret = "127.0.0.1";
		}
		else if (ret.length() > INET_ADDRSTRLEN)
		{
			throw std::exception("Invalid IPv4 address");
		}

		return  ret;
	}
	std::string GetPort()
	{
		std::string ret;
		std::cout << "Enter Server Port or press Enter for use default 7700 \n" << std::endl;
		std::getline(std::cin, ret);

		//TODO check
		if (ret.length() == 0)
		{
			ret = "7700";
		}
		else if (ret.length() > MAX_PORT_DIGIT)
		{
			throw std::exception("Invalid IPv4 port");
		}
		return  ret;
	}
	void PrintMessage(const std::string& str)
	{
		std::cout << str;
	}
	//TODO why can`t I set const & string if i invoce method?
	void PrintMessage(ChatLib::Message& msg)
	{
		std::cout << msg.GetText();
	}
};

	class TCPIP_Client
	{
	public:

		std::string Name;
		std::string ServerIP;
		std::string ServerPort;
		int Socket = NULL;
		
		TCPIP_Client(const std::string& name, const std::string& serverIP, const std::string& serverPort)
		{
			Name = name;
			ServerIP = serverIP;
			ServerPort = serverPort;
		}

		ChatLib::MessageType IntroduceToServer()
		{
			if (Name.length() == 0)
				throw new std::exception("You must fill client.Name before call IntroduceToServer() \n");

			ChatLib::Message message(ChatLib::eNameRequest, Name);

			ChatLib::MessageType msgType;
			do
			{
				msgType = ChatLib::Protocol::TrySendMessage(message, Socket);
			} while (msgType != ChatLib::eResponseOk);
			return msgType;
		}

		void InitializeSocketRoutine()
		{
			WSADATA wsaData;

			if (WSAStartup(MAKEWORD(2, 2), &wsaData))
			{
				std::cout << "Could not initialize winsock \n" << std::endl;
			}

			struct addrinfo hints;
			struct addrinfo *servinfo;

			memset(&hints, 0, sizeof hints);

			hints.ai_family = PF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			//hints.ai_flags = AI_PASSIVE;

			//TODO check errors
			if (int status = getaddrinfo(ServerIP.c_str(), ServerPort.c_str(), &hints, &servinfo) != 0)
			{
				std::cout << "getaddrinfo error: \n" << gai_strerror(status) << std::endl;
			}

			Socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

			//TODO maybe need bind

			//connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
			if (int status = connect(Socket, servinfo->ai_addr, servinfo->ai_addrlen) != 0)
			{
				std::cout << "connect error: " << gai_strerror(status) << std::endl;
			}
		}

		int GetSocket()
		{
			return Socket;
		}
	};

//TODO check
	 int main()
	 {
		 UIInterface ui;
		 std::string confClientName = ui.GetName();
		 std::string confServerIP = ui.GetIP();
		 std::string confServerPort = ui.GetPort();

		 TCPIP_Client client(confClientName, confServerIP, confServerPort);

		 client.InitializeSocketRoutine();
		 int sockfd = client.GetSocket();

		 client.IntroduceToServer();

		 char buff[1024];
		 bool Iasync = false;
		 
		 std::future<std::basic_string<char>> asyncThread;

		 while (true)
		 {
			 //TODO how to delete right
			 ChatLib::Message message = ChatLib::Protocol::TryRecieveMessage(sockfd);

			 if (message.GetText().length() != 0)
				 ui.PrintMessage(message);

			 //TODO what we have in getline if we received a message at the moment when we write a message
			 if (!asyncThread.valid())
			 {
				 asyncThread = std::async(std::launch::async, []
				 {
					 std::string s = "";
					 if (std::getline(std::cin, s)) return s;
				 });
			 }

			 std::string str;
			 if(asyncThread.wait_for(std::chrono::seconds(1)) == std::future_status::ready)
			 {
				 str = asyncThread.get();

				 asyncThread = std::async(std::launch::async, []
				 {
					 std::string s = "";
					 if (std::getline(std::cin, s)) return s;
				 });
			 }
			 else
			 {
				 //str = " Test message \n";
			 }

			 if (str.length())
			 {
				 ChatLib::Message messageForSend(ChatLib::eMessageRequest, client.Name + ": " + str);

				 ChatLib::MessageType response = ChatLib::Protocol::TrySendMessage(messageForSend, sockfd);
			 }
		 }

		 closesocket(sockfd);
		 std::cout << "Hello World!\n";

		 return 0;
	 }

	// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
	// Debug program: F5 or Debug > Start Debugging menu

	// Tips for Getting Started: 
	//   1. Use the Solution Explorer window to add/manage files
	//   2. Use the Team Explorer window to connect to source control
	//   3. Use the Output window to see build output and other messages
	//   4. Use the Error List window to view errors
	//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
	//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file




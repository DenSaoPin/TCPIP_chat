#include "stdafx.h"

#pragma once
#include "TCPIP_CLIENT_DLL.h"
#include "UIInterface.h"

extern "C" TCPIPCLIENTDLL_EXPORT void OnRecieveMessage(char* message);
extern "C" TCPIPCLIENTDLL_EXPORT void ClientMainLoop();
extern "C" TCPIPCLIENTDLL_EXPORT void OnSendMessage();

		TCPIP_Client* TCPIP_Client::_instance = nullptr;

		TCPIP_Client* TCPIP_Client::Instance()
		{
			//TODO add set nullptr
			if (_instance == nullptr)
			{
				_instance = new TCPIP_Client();
			}
			return _instance;

		}

		void TCPIP_Client::Initialize(const std::string& name, const std::string& serverIP, const std::string& serverPort)
		{
			Name = name;
			ServerIP = serverIP;
			ServerPort = serverPort;
		}

		ChatLib::Response TCPIP_Client::IntroduceToServer()
		{
			if (Name.length() == 0)
				throw new std::exception("You must fill pClient->Name before call IntroduceToServer() \n");

			ChatLib::NameRequestMessage NameReqMessage(Name);
			ChatLib::Response response(ChatLib::eResponseInvalid);
			do
			{
				response = ChatLib::Protocol::TrySendMessage(&NameReqMessage, Socket);
			} while (response.GetStatus() != ChatLib::ResponseStatus::eOk);
			return response;
		}

		void TCPIP_Client::InitializeSocketRoutine()
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

		int TCPIP_Client::GetSocket()
		{
			return Socket;
		}

//TODO check
	 void TCPIP_Client::ClientMainLoop()
	 {
		 UIInterface ui;
		 std::string confClientName = ui.GetName();
		 std::string confServerIP = ui.GetIP();
		 std::string confServerPort = ui.GetPort();

		 TCPIP_Client* pClient = Instance();
		 pClient->Initialize(confClientName, confServerIP, confServerPort);

		 pClient->InitializeSocketRoutine();
		 int sockfd = pClient->GetSocket();

		 pClient->IntroduceToServer();

		 char buff[1024];
		 bool Iasync = false;
		 
		 std::future<std::basic_string<char>> asyncThread;

		 while (true)
		 {
			 //TODO how to delete right
			 ChatLib::RawBytes rawData = ChatLib::Protocol::RecieveMessageAndReply(sockfd);

			 if (!rawData.empty())
			 {
				 const ChatLib::MessageType type = ChatLib::BaseMessage::GetType(rawData);
				 ChatLib::byte *p = &rawData[0];
				 std::string text;
				 switch (type)
				 {
				 case ChatLib::eBroadcastMessage:
				 {
					 const ChatLib::BroadcastMessage BroadMessage(p);
					 text = BroadMessage.Text;
					 break;
				 }
				 case ChatLib::eDirectMessage:
				 {
					 const ChatLib::DirectMessage DirectMessage(p);
					 text = DirectMessage.Text;
					 break;
				 }
				 case ChatLib::eResponse:
					 throw std::exception("Main receiving error: I received Response in main");
				 case ChatLib::eNameRequest:
					 throw std::exception("Main receiving error: I received NameRequest in main");
				 default:
					 throw std::exception("Main receiving error: I received unknown message type");
				 }

				 if (text.length() != 0)
					 ui.PrintMessage(text);

				 CallbacksHolder::clbMessageReceive(text.c_str());
			 }
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

			 ChatLib::MessageType messageType = ChatLib::eInvalid;

			 if (str.length())
			 {
				 //TODO shall we improve?
				 int startIndex = str.find("for @");

				 ////TODO how to delete?
				 ChatLib::BaseMessage* message;

				 if(startIndex != std::string::npos)
				 {
					 startIndex += 5;
					 int finishIndex = str.find("@", startIndex);
					 std::string forName = str.substr(startIndex, finishIndex - startIndex);
					 std::string fullMessage = pClient->Name + ": " + str;
					 message = new ChatLib::DirectMessage(forName, fullMessage);
				 }
				 else
				 {
					 message = new ChatLib::BroadcastMessage(str);
				 }

				 ChatLib::Response response = ChatLib::Protocol::TrySendMessage(message, sockfd);

				 delete(message);
				 //TODO response will be always Ok
			 }
		 }

		 closesocket(sockfd);
		 std::cout << "Hello World!\n";
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




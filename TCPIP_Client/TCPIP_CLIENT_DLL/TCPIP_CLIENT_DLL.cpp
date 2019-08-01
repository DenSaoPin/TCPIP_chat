#pragma comment (lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "TCPIP_CLIENT_DLL.h"
#include <ProtocolAPI/BroadcastMessage.h>
#include <ProtocolAPI/DirectMessage.h>
#include <ProtocolAPI/NameRequestMessage.h>
#include "CallbacksHolder.h"
#include "UIInterface.h"



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
		 InitializeSocketRoutine();

		 int sockfd = GetSocket();

		 IntroduceToServer();

		 while (!m_NeedTerminate)
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

				 //if (text.length() != 0)
				 //	 ui.PrintMessage(text);

				 CallbacksHolder::clbMessageReceive(text.c_str());
			 }


			 if(!m_outgoingMessages.empty())
			 { 
				 std::string &textMessage = m_outgoingMessages.front();
				

				 if (textMessage.length() > 0)
				 {

					 ////TODO how to delete?
					 ChatLib::BaseMessage* message;

					 //TODO shall we improve?
					 int startIndex = textMessage.find("for @");
					 if (startIndex != std::string::npos)
					 {
						 startIndex += 5;
						 int finishIndex = textMessage.find("@", startIndex);
						 std::string forName = textMessage.substr(startIndex, finishIndex - startIndex);
						 std::string fullMessage = Name + ": " + textMessage;
						 message = new ChatLib::DirectMessage(forName, fullMessage);
					 }
					 else
					 {
						 message = new ChatLib::BroadcastMessage(textMessage);
					 }

					 ChatLib::Response response = ChatLib::Protocol::TrySendMessage(message, sockfd);

					 //TODO behavior may depend on the response
					 delete(message);
				 }

				 m_outgoingMessages.pop();
			 }
		 }
		 closesocket(sockfd);
		 WSACleanup();
	 }

void TCPIP_Client::SendTextMessage(const char* sz_str)
{
	m_outgoingMessages.push(sz_str);
}

void TCPIP_Client::Shutdown()
{
	m_NeedTerminate = true;
	while (!m_IsTerminate);
}

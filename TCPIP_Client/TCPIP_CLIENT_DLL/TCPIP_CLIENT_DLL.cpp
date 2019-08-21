#pragma comment (lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <mutex>

#include <ProtocolAPI/BroadcastMessage.h>
#include <ProtocolAPI/DirectMessage.h>
#include <ProtocolAPI/NameRequestMessage.h>

#include "ProtocolAPI/Defines.h"

#include "TCPIP_CLIENT_DLL.h"
#include "CallbacksHolder.h"
#include "UIInterface.h"
#include "Exceptions.h"
#include "Defines.h"

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

void TCPIP_Client::ShowError(const std::string& text)
{
	std::string name = "ClientDLL";
	CallbacksHolder::clbMessageReceive(name.c_str(), reinterpret_cast<int*>(ChatLib::eError), text.c_str());
}

bool TCPIP_Client::InitWinSockDll()
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		std::cout << "Could not initialize winsock \n" << std::endl;
		return false;
	}
	return true;
}

bool TCPIP_Client::InitializeSocketRoutine()
{
	struct addrinfo hints;
	struct addrinfo *pServinfo;

	memset(&hints, 0, sizeof hints);

	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//TODO check errors
	if (int status = getaddrinfo(ServerIP.c_str(), ServerPort.c_str(), &hints, &pServinfo) != 0)
	{
		ShowError("getaddrinfo error: \n" + std::string (gai_strerror(status)));
		return false;
	}

	if((Socket = socket(pServinfo->ai_family, pServinfo->ai_socktype, pServinfo->ai_protocol)) == INVALID_SOCKET)
	{
		//TODO add handle WSAErroR and LINUX
		ShowError("create socket error: \n");
		return false;
	};

#ifdef _WIN32
	unsigned long mode = 1; // set as nonblocking socket
	if (const int status = ioctlsocket(Socket, FIONBIO, &mode) != 0)
	{
		//TODO add handle WSAErroR and LINUX
		ShowError("set socket as nonblocking error: \n");
		closesocket(Socket);
		return false;
	}
#else
	if ( int flags = fcntl(Socket F_GETFL, 0)) == -1)
	{
		//TODO add handle LINUX error
		ShowError("set socket as nonblocking error: \n");
		closesocket(Socket);
		return false;
	}
	flags = flags & ~O_NONBLOCK;
	if (int flags = fcntl(Socket F_SETFL, 0)) != 0)
	{
		//TODO add handle LINUX error
		ShowError("set socket as nonblocking error: \n");
		closesocket(Socket);
		return false;
	}
#endif
	int status = 0;
	status = connect(Socket, pServinfo->ai_addr, pServinfo->ai_addrlen);

	//TODO check if
	if ((status == SOCKET_ERROR) && (WSAGetLastError() == WSAEWOULDBLOCK))
	{
		fd_set write, except;

		FD_ZERO(&write);
		FD_SET(Socket, &write);

		FD_ZERO(&except);
		FD_SET(Socket, &except);

		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 500;

		status = select(Socket + 1, NULL, &write, &except, &tv);
		if (status == 0)
		{
			//TODO it is timeout
			WSASetLastError(WSAETIMEDOUT);
			status = SOCKET_ERROR;
		}
		else if (status > 0)
		{
			if (FD_ISSET(Socket, &except))
			{
				int err = 0;
				int size = sizeof(err);
				getsockopt(Socket, SOL_SOCKET, SO_ERROR, (char*)&err, &size);
				WSASetLastError(err);
				status = SOCKET_ERROR;
			}
			else
			{
				//TODO successfully connected
				status = 0;
			}
		}
	}

	if (status == SOCKET_ERROR)
	{
		//TODO handle error...
		closesocket(Socket);
		return false;
	}

	//closesocket(Socket);
	return true;
}

int TCPIP_Client::GetSocket()
{
	return Socket;
}

void TCPIP_Client::ClientMainLoop()
{
	while (!m_NeedTerminate)
	{

		switch (m_ClientStatus)
		{
		case eStartWSA:
			if (InitWinSockDll())
				m_ClientStatus = eInitializeSocket;
			break;
		case eInitializeSocket:
			if (InitializeSocketRoutine())
				m_ClientStatus = eIntroduce;
			break;
		default:
			break;
		}

		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(Socket, &rfds);

		fd_set wfds;
		FD_ZERO(&wfds);
		FD_SET(Socket, &wfds);

		fd_set exfds;
		FD_ZERO(&exfds);
		FD_SET(Socket, &exfds);

		const int maxFD = (int)Socket + 1;

		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 500;
		ChatLib::RawBytes rawData;

		int result = select(maxFD, &rfds, NULL, NULL, &timeout);
		if (result < 0)
		{
			PrintErrors;
			//win errors
			//TODO UNIX check error
		}
		else if (result == 0)
		{
			PrintErrors;
			//TODO win check error and timeout
		}
		else
		{
			switch (m_ClientStatus)
			{
				//case eInvalid:
				//	throw new std::exception(" Invalid client state ");
				//	break;
			case eStartWSA:
			{
				//throw new std::exception(" Invalid client state ");
				break;
			}
			case eInitializeSocket:
			{
				//throw new std::exception(" Invalid client state ");
				break;
			}
			case eIntroduce:
			{
				if (Name.length() == 0)
					throw new std::exception("You must fill pClient->Name before call IntroduceToServer() \n");

				ChatLib::NameRequestMessagePtr NameReqMessagePtr( new ChatLib::NameRequestMessage(Name, GenerateId()));
				m_outgoingMessages.push(NameReqMessagePtr);

				m_ClientStatus = eSendingMessage;
				break;
			}
			case eShutdown:
			{
				break;
			}

			case eReceiveMessage:
			{
				if (FD_ISSET(Socket, &rfds))
				{
					ChatLib::RawBytes rawData = RecieveMessage(Socket);
				

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
							CallbacksHolder::clbMessageReceive(BroadMessage.SourceName.c_str(), reinterpret_cast<int*>(BroadMessage.GetMyType()), BroadMessage.Text.c_str());
							break;
						}
						case ChatLib::eDirectMessage:
						{
							const ChatLib::DirectMessage DirectMessage(p);
							text = DirectMessage.Text;
							break;
						}
						case ChatLib::eResponse:
						{
							const ChatLib::Response Responce(p);

							switch (Responce.GetStatus())
							{
							case ChatLib::ResponseStatus::eOk:
								m_outgoingMessages.pop();
								break;
							case ChatLib::ResponseStatus::eNameConflict:
								break;
							case ChatLib::ResponseStatus::eError:

								break;
							case ChatLib::ResponseStatus::eResponseInvalid:

								break;
							default:
								break;
							}
							break;
						}
						case ChatLib::eNameRequest:
						{
							throw std::exception("Main receiving error: I received NameRequest in main");
							break;
						}
						default:
						{
							throw std::exception("Main receiving error: I received unknown message type");
							break;
						}
						}
					}
				}
				break;
			}
			case eSendingMessage:
			{
				if(FD_ISSET(Socket, &wfds))
				{
					if (SendMessagee(m_outgoingMessages.front(), Socket))
					{
						m_ClientStatus = eReceiveMessage;
					}
					else
					{
						m_ClientStatus = eIntroduce;
					}
				}
				break;
			}
			default:
			{
				break;
			}
			}

		}

		closesocket(GetSocket());
		WSACleanup();
		m_IsTerminated = true;
	}
}
////TODO check
//void TCPIP_Client::MainLoop()
//{
//	InitializeSocketRoutine();
//
//	int sockfd = GetSocket();
//
//	IntroduceToServer();
//
//	m_IsStarted = true;
//
//	while (!m_NeedTerminate)
//	{
//		//TODO how to delete right
//		ChatLib::RawBytes rawData = RecieveMessageAndReply(sockfd);
//
//		if (!rawData.empty())
//		{
//			const ChatLib::MessageType type = ChatLib::BaseMessage::GetType(rawData);
//			ChatLib::byte *p = &rawData[0];
//			std::string text;
//			switch (type)
//			{
//			case ChatLib::eBroadcastMessage:
//			{
//				const ChatLib::BroadcastMessage BroadMessage(p);
//				text = BroadMessage.Text;
//				CallbacksHolder::clbMessageReceive(BroadMessage.SourceName.c_str(), reinterpret_cast<int*>(BroadMessage.GetMyType()), BroadMessage.Text.c_str());
//				break;
//			}
//			case ChatLib::eDirectMessage:
//			{
//				const ChatLib::DirectMessage DirectMessage(p);
//				text = DirectMessage.Text;
//				break;
//			}
//			case ChatLib::eResponse:
//				throw std::exception("Main receiving error: I received Response in main");
//			case ChatLib::eNameRequest:
//				throw std::exception("Main receiving error: I received NameRequest in main");
//			default:
//				throw std::exception("Main receiving error: I received unknown message type");
//			}
//
//			//if (text.length() != 0)
//			//	 ui.PrintMessage(text);
//		}
//
//		if (!m_outgoingMessages.empty())
//		{
//			std::string &textMessage = m_outgoingMessages.front();
//
//
//			if (textMessage.length() > 0)
//			{
//
//				////TODO how to delete?
//				ChatLib::BaseMessage* message;
//
//				//TODO shall we improve?
//				int startIndex = textMessage.find("for @");
//				if (startIndex != std::string::npos)
//				{
//					startIndex += 5;
//					int finishIndex = textMessage.find("@", startIndex);
//					std::string forName = textMessage.substr(startIndex, finishIndex - startIndex);
//					std::string fullMessage = Name + ": " + textMessage;
//					message = new ChatLib::DirectMessage(Name, forName, fullMessage);
//				}
//				else
//				{
//					message = new ChatLib::BroadcastMessage(Name, textMessage);
//				}
//
//				ChatLib::Response response = TrySendMessage(message, sockfd);
//
//				//TODO behavior may depend on the response
//				delete(message);
//			}
//
//			m_outgoingMessages.pop();
//		}
//	}
//	closesocket(sockfd);
//	WSACleanup();
//
//	m_IsTerminate = true;
//}
//
void TCPIP_Client::AddForSend(const char* sz_target_name, const int messageType, const void* data, const int data_len)
{
	const auto type = static_cast<ChatLib::MessageType>(messageType);

	switch (type)
	{
	case ChatLib::MessageType::eInvalid:
		throw new std::exception("Invalid message type");
		break;
	case ChatLib::MessageType::eBroadcastMessage:
	{

		//const char * text = (const char *)data;
		//size_t len = data_len * sizeof(char);
		//std::string data(text, len);

		const ChatLib::BroadcastMessagePtr broadPtr(new ChatLib::BroadcastMessage(Name, std::string((const char *)data, data_len * sizeof(char)), GenerateId()));
		m_outgoingMessages.push(broadPtr);
		break;
	}
	case ChatLib::MessageType::eDirectMessage:
	{
		const ChatLib::DirectMessagePtr directPtr(new ChatLib::DirectMessage(Name, std::string(sz_target_name), std::string((const char *)data, data_len * sizeof(char)), GenerateId()));
		m_outgoingMessages.push(directPtr);
		break;
	}
	case ChatLib::MessageType::eNameRequest:
	{
		const ChatLib::NameRequestMessagePtr nameReqPtr(new ChatLib::NameRequestMessage(Name, GenerateId()));
		m_outgoingMessages.push(nameReqPtr);
		break;
	}
	case ChatLib::MessageType::eResponse:
		throw new std::exception(" Cathed Response from UI \n");
		break;
	}
}

unsigned short TCPIP_Client::GenerateId()
{
	return ++m_currentMessageId;
}

void TCPIP_Client::Shutdown()
{
	m_NeedTerminate = true;
	while (!m_IsTerminated);
}

ChatLib::RawBytes TCPIP_Client::RecieveMessage(const CROSS_SOCKET& socket)
{
		//TODO check recieved num
		char buff[MAX_PACKAGE_LENGTH];
		const int recived = recv(socket, buff, MAX_PACKAGE_LENGTH, NULL);

		if (recived == 0)
		{
			PrintErrors;
			printf("Recieve message: connection softly closed");
			throw Exceptions::ConnectionClosedException("connection softly closed");
			//TODO error check Win and Timeout
		}
		else if (recived == -1)
		{
			PrintErrors;
			throw Exceptions::ConnectionLostException("connection lost closed");
			//WSACleanup();
			//TODO check Unix error
		}

		printf("RecieveMessage %d bytes: ", recived);

		//TODO check raw data
		ChatLib::RawBytes rawData(buff, buff + recived);
		for (int i = 0; i < recived; i++)
		{
			printf("%02X ", buff[i]);
		}
		printf("\n");

		printf("Message was recieved \n");

		return rawData;
}

bool TCPIP_Client::SendMessagee(ChatLib::BaseMessagePtr message, const CROSS_SOCKET& socket)
{
	fd_set wfds;
	FD_ZERO(&wfds);
	FD_SET(socket, &wfds);
	int maxFD = (int)socket + 1;

	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 500;
	ChatLib::RawBytes rawData;

	const int result = select(maxFD, NULL, &wfds, NULL, &timeout);
	if (result < 0)
	{
		PrintErrors;
		//win errors
		//TODO UNIX check error
		return false;
	}
	else if (result == 0)
	{
		PrintErrors;
		//TODO win check error and timeout
		return false;
	}
	else
	{
		byte pBuffer[255];
		int length = message->Construct(pBuffer);

		//TODO should we use while
		int sended = 0;
		do
		{
			sended = send(socket, (char *)&pBuffer[sended], length, NULL);
			length -= sended;
		} while (length > 0);

		return true;
	}
}

bool TCPIP_Client::GetStatus()
{
	return _instance->m_IsStarted && !_instance->m_IsTerminated;
}

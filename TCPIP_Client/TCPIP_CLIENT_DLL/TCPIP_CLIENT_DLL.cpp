#pragma comment (lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <mutex>
#include <chrono>

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
	using Time = std::chrono::steady_clock;
	using ms = std::chrono::milliseconds;
	using int_sec = std::chrono::duration<int>;

    m_ClientStatus = eStartWSA;

    if (m_ClientStatus == eStartWSA)
    {
        if (InitWinSockDll())
            m_ClientStatus = eInitializeSocket;
        else
        {
            throw new std::exception(" Start WSA failed \n");
        }
    }

    if (m_ClientStatus == eInitializeSocket)
    {
        if (InitializeSocketRoutine())
            m_ClientStatus = eIntroduce;
        else
        {
            throw new std::exception(" Initialize socket failed \n");
        }
    }
	Time::time_point lapStartTime;
	const int_sec timeGap{ 76 };   
	while (true)
	{
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

		int result = select(maxFD, &rfds, &wfds, &exfds, &timeout);
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
			case eStartWSA:
			{
				throw new std::exception(" Invalid client state ");
				break;
			}
			case eInitializeSocket:
			{
				throw new std::exception(" Invalid client state ");
				break;
			}
			case eIntroduce:
			{
				if (Name.length() == 0)
					throw new std::exception("You must fill pClient->Name before call IntroduceToServer() \n");

				ChatLib::NameRequestMessagePtr NameReqMessagePtr(new ChatLib::NameRequestMessage(Name, GenerateId()));
                m_NameRequest = NameReqMessagePtr;
                    
				m_ClientStatus = eIdle;
				break;
			}
			case eIdle:
            {
                if (FD_ISSET(Socket, &rfds))
                {
                    ChatLib::RawBytes rawData;
                    rawData = RecieveMessage(Socket);

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
                            CallbacksHolder::clbMessageReceive(BroadMessage.SourceName.c_str(), reinterpret_cast<int*>(BroadMessage.GetMyType()), BroadMessage.Text.c_str());

                            SetResponse(ChatLib::eOk, BroadMessage.GetMyID());
                            break;
                        }
                        case ChatLib::eDirectMessage:
                        {
                            const ChatLib::DirectMessage DirectMessage(p);
                            CallbacksHolder::clbMessageReceive(DirectMessage.SourceName.c_str(), reinterpret_cast<int*>(DirectMessage.GetMyType()), DirectMessage.Text.c_str());

                            SetResponse(ChatLib::eOk, DirectMessage.GetMyID());
                            break;
                        }
                        case ChatLib::eResponse:
                        {
                            const ChatLib::Response Responce(p);

                            switch (Responce.GetStatus())
                            {
                            case ChatLib::ResponseStatus::eOk:
                                if ((!m_outgoingMessages.empty()) && m_outgoingMessages.front()->GetMyID() == Responce.GetMyID())
                                    m_outgoingMessages.pop();
                                if ((m_NameRequest != nullptr) && (m_NameRequest->GetMyID() == Responce.GetMyID()))
                                    m_NameRequest = nullptr;
                                break;
                            case ChatLib::ResponseStatus::eNameConflict:
                            {
                                text = " Name already in use \n";
                                CallbacksHolder::clbMessageReceive(Name.c_str(), reinterpret_cast<int*>(Responce.GetMyType()), text.c_str());
                                break;
                            }
                            case ChatLib::ResponseStatus::eError:
                            {
                                text = " Unhandled error response ";
                                CallbacksHolder::clbMessageReceive(Name.c_str(), reinterpret_cast<int*>(Responce.GetMyType()), text.c_str());
                                break;
                            }
                            case ChatLib::ResponseStatus::eResponseInvalid:
                            {
                                text = " Unhandled invalid response ";
                                CallbacksHolder::clbMessageReceive(Name.c_str(), reinterpret_cast<int*>(Responce.GetMyType()), text.c_str());
                                break;
                            }
                            default:
                                text = " Unhandled default response ";
                                CallbacksHolder::clbMessageReceive(Name.c_str(), reinterpret_cast<int*>(Responce.GetMyType()), text.c_str());
                                break;
                            }
                            break;
                        }
                        case ChatLib::eNameRequest:
                        {
                            const ChatLib::NameRequestMessage NameRequestMessage(p);
                            text = " Error: catched nameRequest in main ";
                            CallbacksHolder::clbMessageReceive(Name.c_str(), reinterpret_cast<int*>(NameRequestMessage.GetMyType()), text.c_str());
                            break;
                        }
                        default:
                        {
                            text = " Error: catched nameRequest in main ";
                            CallbacksHolder::clbMessageReceive(Name.c_str(), reinterpret_cast<int*>(ChatLib::eError), text.c_str());
                            break;
                        }
                        }
                    }
                }

                if(m_NameRequest != nullptr || !m_outgoingMessages.empty())
                {
                    ChatLib::BaseMessagePtr currentMessage;
                    if (m_NameRequest != nullptr &&
                        FD_ISSET(Socket, &wfds))
                    {
                        currentMessage = m_NameRequest;
                    }
                    else if ((!m_outgoingMessages.empty()) &&
                        FD_ISSET(Socket, &wfds))
                    {
                        currentMessage = m_outgoingMessages.front();
                    }

                    if (currentMessage == awaitResponse)
                    {
                        if (Time::now() > lapStartTime + timeGap)
                        {
                            SendMessagee(currentMessage, Socket);
                            awaitResponse = currentMessage;
                            lapStartTime = Time::now();
                        }
                    }
                    else
                    {
                        SendMessagee(currentMessage, Socket);
                        awaitResponse = currentMessage;
                    }

                    if ((!m_outgoingMessages.empty()) && (m_outgoingMessages.front()->GetMyType() == ChatLib::eResponse))
                        m_outgoingMessages.pop();
                }
                break;
            }
			case eTerminating:
			{
				closesocket(GetSocket());
				WSACleanup();
				m_ClientStatus = eShutDown;
				break;
			}
				default:
			{
				std::string text = " Unhandled default case in main \n";
				std::string name = " ClientDll";
				CallbacksHolder::clbMessageReceive(name.c_str(), reinterpret_cast<int*>(ChatLib::eError), text.c_str());
				break;
			}
			}

			if(m_ClientStatus == eShutDown)
				break;
		}
	}
}

void TCPIP_Client::AddForSend(std::string& sz_target_name, const int messageType, const void* data, const int data_len)
{
	const auto type = static_cast<ChatLib::MessageType>(messageType);

	switch (type)
	{
	case ChatLib::MessageType::eInvalid:
		throw new std::exception("Invalid message type");
		break;
	case ChatLib::MessageType::eBroadcastMessage:
	{
		const ChatLib::BroadcastMessagePtr broadPtr(new ChatLib::BroadcastMessage(Name, std::string((const char *)data, data_len * sizeof(char)), GenerateId()));
		m_outgoingMessages.push(broadPtr);
		break;
	}
	case ChatLib::MessageType::eDirectMessage:
	{
		const ChatLib::DirectMessagePtr directPtr(new ChatLib::DirectMessage(Name, sz_target_name, std::string((const char *)data, data_len * sizeof(char)), GenerateId()));
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
    if(m_ClientStatus != eShutDown)
	    m_ClientStatus = eTerminating;

	while (m_ClientStatus != eShutDown);
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

EClientStatus TCPIP_Client::GetStatus()
{
	return m_ClientStatus;
}

void TCPIP_Client::SetResponse(ChatLib::ResponseStatus status, const unsigned short &id)
{
	const ChatLib::ResponsePtr responsePtr(new ChatLib::Response(status, id));
	m_outgoingMessages.push(responsePtr);
}
#include "ServerClient.h"
#include <iostream>
#include <WS2tcpip.h>
#include "LoggerManager.h"
#include "ProtocolAPI/Defines.h"
#include "Exceptions.h"
#define MAX_LENGTH 1024

#if defined _WIN32
#include <WinSock2.h>
#define PrintErrors printWsaError()
#define CROSS_SOCKET SOCKET
#else
#define PrintErrors printLinuxError()
#endif

const char * DefaultAddress = "0.0.0.0";

TCPServer::TCPServer(std::string address, std::string port)
{
	m_log = LoggerManager::GetLogger("ServerLog");
	m_log->error("test%d", 1);
	_connectSock = 0;
	_clientCount = 0;
	ServerIP = address;
	ServerPort = port;

	InitialWSARoutine();
	ListenSockInitialization(address, port);
}

bool TCPServer::Assign(const std::string& name, ServerClient* pClient)
{
	//TODO check
	if (Clients.Check(name))
	{
		return false;
	}

	pClient->Name = name;
	_iknowThisNames.insert(name);
	return true;
	//pClient->Name = name;

	//return Clients.Find(name);
}

bool TCPServer::SetToSendForAllClients(ServerClient* server_client, ChatLib::BroadcastMessagePtr& broadMessagePtr)
{
	for (auto it = this->Clients.Begin(); it != this->Clients.End(); ++it)
	{
		if (*it == server_client)
		{
			continue;
		}

		(*it)->ForSend.push(broadMessagePtr);
	}
	return true;
}

void TCPServer::SetToSendFor(ServerClient * server_client, ChatLib::DirectMessagePtr& directMessagePtr)
{
	for(auto it = this->Clients.Begin(); it != this->Clients.End(); ++it)
	{
		if (*it == server_client)
		{
			(*it)->ForSend.push(std::static_pointer_cast<ChatLib::BaseMessage>(directMessagePtr));
		}
	}
}

void TCPServer::InitialWSARoutine()
{
	char buffer[MAX_LENGTH];

	memset(buffer, ' ', MAX_LENGTH);

	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);

	int wsaInitError = WSAStartup(wVersion, &wsaData);

	if (wsaInitError != 0)
	{
		std::cout << "Could not initialize winsock" << std::endl;
	}

	if (wsaData.wVersion != 514)
	{
		//CheckSockError(GetLastError());
	}
}

void TCPServer::ListenSockInitialization(std::string& IPv4_Adress,std::string& port)
{
	struct addrinfo hints, *p;
	struct addrinfo* pResults;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = PF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	//TODO why res?
	int res;

	if (IPv4_Adress.length() == 0)
	{
		IPv4_Adress.push_back(0);
	}
	if ((res = getaddrinfo(IPv4_Adress.c_str(), port.c_str(), &hints, &pResults)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %ws\n", gai_strerror(res));
	}

	for (p = pResults; p != NULL; p = p->ai_next)
	{
		if ((ListenSock = socket(p->ai_family, p->ai_socktype,
		                         p->ai_protocol)) == -1)
		{
			printf("server: socket  error \n");
			continue;
		}
		//TODO check
		char yes = '1';
		if (setsockopt(ListenSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
		{
			printf("server: setsockopt error \n");
			exit(1);
		}
		if (bind(ListenSock, p->ai_addr, (int)p->ai_addrlen) == -1)
		{
			closesocket(ListenSock);
			printf("server: bind  error \n");
			continue;
		}
		break;
	}

	if (p == NULL)
	{
		fprintf(stderr, "server: failed to bind \n");
	}

	if (listen(ListenSock, 10) == -1)
	{
		printf("listen");
		exit(1);
	}

	m_log->info("start");
	m_log->info("listen %s:%s", IPv4_Adress.c_str(), port.c_str());
	char m_connect[] = "_connectSock to server \n";

	freeaddrinfo(pResults);
}

void TCPServer::Accept()
{
    fd_set rfds;

	FD_ZERO(&rfds);
	FD_SET(ListenSock, &rfds);

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1;

	int result = select(ListenSock + 1, &rfds, NULL, NULL, &tv);

    if (result == 0)
    {
        //TODO timeout, it is normal case
    }
    else if (result == SOCKET_ERROR)
    {
        PrintErrors;
    }
    else
    {
        if (FD_ISSET(ListenSock, &rfds))
        {
            struct sockaddr_storage their_addr;

            socklen_t addr_size = sizeof their_addr;

            CROSS_SOCKET result = accept(ListenSock, (struct sockaddr*)&their_addr, &addr_size);
            if (result <= 0)
            {
                printf("server: accept error \n");
                //TODO error check
            }
            else
            {
                printf("Client was connected \n");
                //TODO check!!!!!
                Clients.Add(new ServerClient(this, result));
            }
        }
    }
}

void TCPServer::SendMessagee(ChatLib::BaseMessage* message, const CROSS_SOCKET& socket)
{
	byte pBuffer[255];
	int length = message->Construct(pBuffer);

	int sended = 0;
	do
	{
		sended = send(socket, (char *)&pBuffer[sended], length, NULL);
		length -= sended;
	} while (length > 0);
}

void TCPServer::SetResponse(ChatLib::ResponseStatus status, ServerClient& client, const unsigned short &id)
{
	const ChatLib::ResponsePtr responsePtr(new ChatLib::Response(status, id));
	client.ForSend.push(responsePtr);
}

ChatLib::RawBytes TCPServer::RecieveMessage(const CROSS_SOCKET& socket)
{
	//TODO check recieved num
	char buff[MAX_PACKAGE_LENGTH];
	const int recived = recv(socket, buff, MAX_PACKAGE_LENGTH, NULL);

	if (recived == 0)
	{
		printf(" Recieve message: connection softly closed \n");
		throw Exceptions::ConnectionClosedException(" Connection softly closed \n");
		//TODO error check Win and Timeout
	}
	else if (recived == -1)
	{
		PrintErrors;
		closesocket(socket);
        printf(" Recieve message: connection lost closed \n");
		throw Exceptions::ConnectionLostException(" Connection lost closed \n");
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

	//MessageType type = BaseMessage::GetType(buff);

	//printf("Recieved message type equal %d \n", type);

	////Message message (type, buff);

	//printf("Recieved message equal %s \n", message.GetText());

	return rawData;
}


#include "Server.h"
#include "ServerClient.h"
#include <iostream>
#include <WS2tcpip.h>

#define MAX_LENGTH 1024

const char * Server::DefaultAddress = "127.0.0.1";

Server::Server(std::string address, std::string port)
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

bool Server::Assign(const std::string& name, ServerClient* pClient)
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

bool Server::SetToSendForAllClients(ServerClient* server_client, ChatLib::BroadcastMessagePtr& broadMessagePtr)
{
	for (auto it = this->Clients.Begin(); it != this->Clients.End(); ++it)
	{
		if (*it == server_client)
		{
			continue;
		}

		(*it)->ForSend.push(std::static_pointer_cast<ChatLib::BaseMessage>(broadMessagePtr));
	}
	return true;
}

void Server::SetToSendFor(ServerClient * server_client, ChatLib::DirectMessagePtr& directMessagePtr)
{

	for(auto it = this->Clients.Begin(); it != this->Clients.End(); ++it)
	{
		if (*it == server_client)
		{
			(*it)->ForSend.push(std::static_pointer_cast<ChatLib::BaseMessage>(directMessagePtr));
		}
	}
}
//
//ServerClient* Server::GetClientByName(std::string& name)
//{
//	if(_iknowThisNames.find(name) != _iknowThisNames.end())
//		 return _iknowThisNames[name];
//	return nullptr;
//}

void Server::InitialWSARoutine()
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

void Server::ListenSockInitialization(std::string& IPv4_Adress,std::string& port)
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

void Server::Accept()
{
	fd_set rfds;

	FD_ZERO(&rfds);
	FD_SET(ListenSock, &rfds);

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1;

	int result = select(ListenSock + 1, &rfds, NULL, NULL, &tv);

	//TODO errors! -1

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

// TCPIP_Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once

#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <thread>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "../../ProtocolAPI/ProtocolAPI/Protocol.h"
#include "../../ProtocolAPI/ProtocolAPI/Message.h"
#include <unordered_map>
#include <queue>

//#define SOCKET_ERROR (-1)
#define MAX_CLIENTS 10
#define MAX_LENGTH 1024
#define HEADER_START 0xFFFFFFFF
//#define MESSAGE_START_INDEX 7
//#define RESPONSE_OK_LENGTH 7
int wsaError = 0;

void CheckSockError(int wsaError);
static inline char* rtrim(char* buffer);
void printWsaError();

//class Connecton
//{
//	std::string ClientName;
//	int ClientSocket;
//};


class Clients
{
	std::string Name;
	CROSS_SOCKET Socket;
};

class ServerTCPIP
{
private:
	int ConnectSock;
	std::string strNameClient;
	int ClientCount = 0;
public:
	std::string ServerIP = "127.0.0.1";
	std::string ServerPort = "7700";

	SocketsVector ConnectionSocketsVec;
	CROSS_SOCKET ListenSock = 0;

	std::unordered_map<std::string, CROSS_SOCKET> mapClientNames;


	void InitialWSARoutine()
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

	void SetListenSock(std::string& IPv4_Adress, std::string& port)
	{
		struct addrinfo hints, *p;
		struct addrinfo *pResults;

		ZeroMemory(&hints, sizeof(hints));

		hints.ai_family = PF_INET;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		//TODO why res?
		int res;

		if(IPv4_Adress.length() == 0)
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
			if (bind(ListenSock, p->ai_addr, (int)p->ai_addrlen) == -1) {
				closesocket(ListenSock);
				printf("server: bind  error \n");
				continue;
			}
			break;
		}

		if (p == NULL) {
			fprintf(stderr, "server: failed to bind \n");
		}

		if (listen(ListenSock, 10) == -1) {
			printf("listen");
			exit(1);
		}

		printf("Start server\n");
		char m_connect[] = "ConnectSock to server \n";

		freeaddrinfo(pResults);
	}

	int SetAllConections(fd_set* rfds, SocketsVector* connectionsVec = nullptr)
	{
		FD_ZERO(rfds);
		FD_SET(ListenSock, rfds);

		int maxFD = (int)ListenSock + 1;

		if(connectionsVec != nullptr)
		{
			for (SocketsVectorIterator it = connectionsVec->begin(); it != connectionsVec->end(); ++it)
			{
				//TODO check s is ref?
				//TODO maybe add in vector at this place
				CROSS_SOCKET s = *it;
				FD_SET(s, rfds);
				maxFD = maxFD > s ? maxFD : s + 1;
			}
		}
		else
		{
			for (SocketsVectorIterator it = ConnectionSocketsVec.begin(); it != ConnectionSocketsVec.end(); ++it)
			{
				//TODO check s is ref?
				//TODO maybe add in vector at this place
				CROSS_SOCKET s = *it;
				FD_SET(s, rfds);
				maxFD = maxFD > s ? maxFD : s + 1;
			}
		}


		return maxFD;
	}

};


int main()
{
	std::unordered_map<std::string, CROSS_SOCKET>::iterator itNameMap;
	std::unordered_map<std::string, CROSS_SOCKET> mapClientNames;
	//std::unordered_map<std::vector<int>::iterator, std::unordered_map<std::string, int>::iterator> clientsToDelete;
	std::vector<SocketsVectorIterator> clientsToDelete;

	std::string strNameClient;

	ServerTCPIP server;
	server.InitialWSARoutine();
	server.SetListenSock(server.ServerIP, server.ServerPort);

	for (;; Sleep(75))
	{
		fd_set rfds;

		//SocketsVector* vec = new SocketsVector;

		int maxFD = server.SetAllConections(&rfds);

		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 500;

		int count = select(maxFD, &rfds, NULL, NULL, &timeout);

		struct sockaddr_storage their_addr;

		socklen_t addr_size = sizeof their_addr;

		if (count)
		{
			SocketsVectorIterator it;

			for (it = server.ConnectionSocketsVec.begin(); it != server.ConnectionSocketsVec.end() && count; it++)
			{
				if (FD_ISSET(*it, &rfds))
				{
					--count;

					ChatLib::Message message = ChatLib::Protocol::RecieveMessage(*it);

					int nNameLen = message.GetTextLength();
					if (message.GetText() != "")
					{
						switch (message.GetType())
						{
						case ChatLib::eNameRequest:

							strNameClient.erase();

							strNameClient = message.GetText();

							if (mapClientNames.find(strNameClient) != mapClientNames.end())
							{
								ChatLib::Message msg(ChatLib::eMessageRequest, "Name " + strNameClient + " already used");

								ChatLib::Protocol::TrySendMessage(msg, *it);
							}
							else
							{
								mapClientNames.insert(std::make_pair(strNameClient, *it));

								printf("Name Responce \n");
								ChatLib::Protocol::SendResponse(ChatLib::eResponseOk, *it);
							}
							break;

						case ChatLib::eMessageRequest:

							//int nMessageLength = message.GetTextLength();

							for (itNameMap = mapClientNames.begin(); itNameMap != mapClientNames.end(); itNameMap++)
							{
								if (itNameMap->second == *it)
								{
									std::cout << itNameMap->first + ": " + message.GetText() << std::endl;
									//TODO send responce;

									printf("Message Responce \n");
									ChatLib::Protocol::SendResponse(ChatLib::eResponseOk, *it);
								}
								else
								{
									std::cout << "Error: Dont know this user name" << std::endl;
								}
							}

							std::cout << "Message recieved: " + message.GetText() << std::endl;
							break;
							case ChatLib::eResponseOk:
							case ChatLib::eResponceError:
								std::cout << "Responce in Main" << std::endl;
							break;
							case ChatLib::eInvalid:
							break;
						}
						std::cout << "Byte recieved: " << message.GetTextLength() << std::endl;
					}
				}
				else
				{
					clientsToDelete.push_back(it);
				}
			}
		}

		for (auto clients_to_delete : clientsToDelete)
		{
			server.ConnectionSocketsVec.erase(clients_to_delete);
		}

		if (FD_ISSET(server.ListenSock, &rfds))
		{
			CROSS_SOCKET result = accept(server.ListenSock, (struct sockaddr*)&their_addr, &addr_size);
			if (result <= 0)
			{
				printf("server: accept error \n");
			}
			else
			{
				printf("Client was connected \n");
				server.ConnectionSocketsVec.push_back(result);
			}
		}

		else if (count < 0)
		{
			//TODO UNIX error
			printf("server: select has UNIX error \n");
		}
		else
		{
			//TODO Win error or 
			//printf("server: select has WIN error \n");
		}
	}
}


void CheckSockError(int wsaError)
{
	switch (wsaError)
	{
	case WSANOTINITIALISED:
	{
		std::cout << "Call WSAStartup() first" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAENETDOWN:
	{
		std::cout << "WSAENETDOWN" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAEAFNOSUPPORT:
	{
		std::cout << "WSAEAFNOSUPPORT" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAEINPROGRESS:
	{
		std::cout << "WSAEINPROGRESS - Blocking call" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAEFAULT:
	{
		std::cout << "WSAEFAULT" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAEINVAL:
	{
		std::cout << "WSAEINVAL" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAEINVALIDPROVIDER:
	{
		std::cout << "WSAEINVALIDPROVIDER" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAEINVALIDPROCTABLE:
	{
		std::cout << "WSAEINVALIDPROCTABLE" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAEMFILE:
	{
		std::cout << "WSAEMFILE - Probably memory leack" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAENOBUFS:
	{
		std::cout << "WSAENOBUFS - Probably memory leack" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAEPROTONOSUPPORT:
	{
		std::cout << "WSAEPROTONOSUPPORT" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAEPROVIDERFAILEDINIT:
	{
		std::cout << "WSAEPROVIDERFAILEDINIT" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAEPROTOTYPE:
	{
		std::cout << "WSAEPROTOTYPE" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAESOCKTNOSUPPORT:
	{
		std::cout << "WSAESOCKTNOSUPPORT" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAENOTSOCK:
	{
		std::cout << "WSAENOTSOCK - the descriptor is no socket" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAEINTR:
	{
		std::cout << "WSAEINTR call was canceled" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAEWOULDBLOCK:
	{
		std::cout << "WSAEWOULDBLOCK - the socket marked nonblocking" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAEADDRINUSE:
	{
		std::cout << "WSAEADDRINUSE - The socket's local address is already in use and the socket was not marked to allow address" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAEISCONN:
	{
		std::cout << "WSAEISCONN - The socket is already connected." << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	case WSAEOPNOTSUPP:
	{
		std::cout << "WSAEOPNOTSUPP - The referenced socket is not of a type that supports" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	default:
	{
		std::cout << "Unknown socket error" << std::endl;
		WSACleanup();
		exit(1);
		break;
	}
	}
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

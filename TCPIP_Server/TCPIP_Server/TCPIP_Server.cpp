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

#if defined _WIN
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#endif


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
class Connecton
{
	std::string ClientName;
	int ClientSocket;

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

	std::vector<int> ConnectionSocketsVec;


	int ListenSock = 0;


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

	void SetListenSock(std::string IPv4_Adress, std::string port)
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
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(res));
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
			if (bind(ListenSock, p->ai_addr, p->ai_addrlen) == -1) {
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
};

int main()
{
	char buffer[125];
	std::unordered_map<std::string, int>::iterator itNameMap;
	std::unordered_map<std::string, int> mapClientNames;
	std::string strNameClient;

	ServerTCPIP server;
	server.InitialWSARoutine();
	server.SetListenSock(server.ServerIP, server.ServerPort);

	for (;; Sleep(75))
	{
		std::vector<int>::iterator it;
		std::vector<int>::iterator anotherIt;

		fd_set exeptfds, rfds;
		//FD_ZERO(&exeptfds);
		//FD_SET(0, &exeptfds);

		FD_ZERO(&rfds);
		FD_SET(server.ListenSock, &rfds);

		int maxFD = server.ListenSock;

		for (it = server.ConnectionSocketsVec.begin(); it != server.ConnectionSocketsVec.end(); ++it)
		{
			//TODO check s is ref?
			//TODO maybe add in vector at this place
			int s = *it;
			FD_SET(s, &rfds);
			//TODO maxFD wil be grow more and more
			maxFD = max(maxFD, s) + 1;
		}


		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 500;

		int count = select(maxFD, &rfds, NULL, NULL, &timeout);

		struct sockaddr_storage their_addr;

		socklen_t addr_size = sizeof their_addr;
		std::vector<std::vector<int>::iterator> clientsToDelete;

		if (count)
		{
			for (it = server.ConnectionSocketsVec.begin(); it != server.ConnectionSocketsVec.end() && count; it++)
			{
				//IHaveIncomingMessage()
				if (FD_ISSET(*it, &rfds))
				{
					--count;

					//memset(buffer, 0, sizeof buffer);

					//TODO check
					//ChatLib::Message message = ChatLib::Protocol::TryRecieveMessage(*it);
					ChatLib::Message message = ChatLib::Protocol::RecieveMessage(*it);

					//TODO if message is empty or not legal?
					ChatLib::Protocol::SendResponse(ChatLib::eResponseOk, *it);

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
								ChatLib::Message msg("Name " + strNameClient + " already used");

								ChatLib::Protocol::TrySendMessage(msg, *it);
							}
							else
							{
								mapClientNames.insert(std::make_pair(strNameClient, *it));

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
			int result = accept(server.ListenSock, (struct sockaddr*)&their_addr, &addr_size);
			if (result <= 0)
			{
				printf("server: accept error \n");
			}
			else
			{
				printf("Client was connected \n");
				int s = result;
				server.ConnectionSocketsVec.push_back(s);
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


void printWsaError()
{
	switch (WSAGetLastError())
	{
	case WSANOTINITIALISED: printf("WSANOTINITIALISED: A successful WSAStartup call must occur before using this function.\n"); break;

	case WSAENETDOWN: printf("WSAENETDOWN: The network subsystem has failed.\n"); break;

	case WSAEFAULT: printf("WSAEFAULT: The buf parameter is not completely contained in a valid part of the user address space.\n"); break;

	case WSAENOTCONN: printf("WSAENOTCONN: The socket is not connected.\n"); break;

	case WSAEINTR: printf("WSAEINTR: The (blocking) call was canceled through WSACancelBlockingCall.\n"); break;

	case WSAEINPROGRESS: printf("WSAEINPROGRESS: A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.\n"); break;

	case WSAENETRESET: printf("WSAENETRESET: For a connection-oriented socket, this error indicates that the connection has been broken due to keep-alive activity that detected a failure while the operation was in progress. For a datagram socket, this error indicates that the time to live has expired.\n"); break;

	case WSAENOTSOCK: printf("WSAENOTSOCK: The descriptor is not a socket.\n"); break;

	case WSAEOPNOTSUPP: printf("WSAEOPNOTSUPP: MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations.\n"); break;

	case WSAESHUTDOWN: printf("WSAESHUTDOWN: The socket has been shut down; it is not possible to receive on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH.\n"); break;

	case WSAEWOULDBLOCK: printf("WSAEWOULDBLOCK: The socket is marked as nonblocking and the receive operation would block.\n"); break;

	case WSAEMSGSIZE: printf("WSAEMSGSIZE: The message was too large to fit into the specified buffer and was truncated.\n"); break;

	case WSAEINVAL: printf("WSAEINVAL: The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative.\n"); break;

	case WSAECONNABORTED: printf("WSAECONNABORTED: The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.\n"); break;

	case WSAETIMEDOUT: printf("WSAETIMEDOUT: The connection has been dropped because of a network failure or because the peer system failed to respond.\n"); break;

	case WSAECONNRESET: printf("WSAECONNRESET: The virtual circuit was reset by the remote side executing a hard or abortive close. The application should close the socket as it is no longer usable. On a UDP-datagram socket, this error would indicate that a previous send operation resulted in an ICMP \"Port Unreachable\" message.\n"); break;
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

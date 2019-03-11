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
#include "ServerClient.h"
#include "Server.h"

#define MAX_CLIENTS 10
#define MAX_LENGTH 1024
#define HEADER_START 0xFFFFFFFF
int wsaError = 0;

void CheckSockError(int wsaError);
static inline char* rtrim(char* buffer);
void printWsaError();

int main()
{
	std::string strNameClient;

	Server server("127.0.0.1", "7700");

	for (;; Sleep(75))
	{
		server.Accept();

		std::vector<ServerClient *> clientsToDelete;
		for (auto it = server.Clients.Begin(); it != server.Clients.End(); ++it)
		{
			(*it)->ProcessSocket();
			if ((*it)->IInvalid)
				clientsToDelete.push_back(*it);
		}

		for (auto clients_to_delete : clientsToDelete)
		{
			server.Clients.Delete(clients_to_delete);
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

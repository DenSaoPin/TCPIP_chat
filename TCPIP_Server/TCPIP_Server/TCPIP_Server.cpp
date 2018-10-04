// TCPIP_Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
# pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <iostream>

#include <thread>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#if defined _WIN
#include <WinSock2.h>
#include <WS2tcpip.h>
static inline int poll(struct pollfd *pfd, int nfds, int timeout) { return WSAPoll(pfd, nfds, timeout); }
#else
#include <poll.h>
//TODO
#endif


//#define SOCKET_ERROR (-1)
#define MAX_CLIENTS 10

SOCKET Connect;
SOCKET *Connections;
SOCKET Listen;

int ClientCount = 0;
char* buffer = new char[1024];

int wsaError = 0;
bool close_conn = false;
bool end_server = false;
bool compress_array = false;

void CheckSockError(int wsaError);

void SendAll(int id)
{
	char* buffer = new char[1024];
	for (;; Sleep(75))
	{
		memset(buffer, 0, sizeof(buffer));
		if (recv(Connections[id], buffer, 1024, NULL))
		{
			printf(buffer);
			printf("\n");

			for (int i = 0; i <= ClientCount; i++)
			{
				send(Connections[i], buffer, sizeof(buffer), NULL);
			}
		}
	}
	delete[] buffer;
}

//void *get_in_addr(struct sockaddr *sa)
//{
//	if (sa->sa_family == AF_INET) {
//		return &(((struct sockaddr_in*)sa)->sin_addr);
//	}
//
//	return &(((struct sockaddr_in6*)sa)->sin6_addr);
//}


int main()
{
	SOCKET retval = 0;

	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);

	int wsaInitError = WSAStartup(wVersion, &wsaData);

	if (wsaInitError != 0)
	{
		std::cout << "Could not initialize winsock" << std::endl;
	}

	if (wsaData.wVersion != 514)
	{
		CheckSockError(GetLastError());
	}

	struct addrinfo hints;
	struct addrinfo *pResults, *p;

	Connections = (SOCKET*)calloc(64, sizeof(SOCKET));

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = PF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	SOCKET res;
	if ((res = getaddrinfo(NULL, "7700", &hints, &pResults)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(res));
		return 1;
	}

	if ((Listen = socket(pResults->ai_family, pResults->ai_socktype,
		pResults->ai_protocol)) == -1)
	{
		perror("server: socket");
	}

	//retval = setsockopt(Listen, SOL_SOCKET, SO_REUSEADDR,
	//	(char *)&on, sizeof(on));
	//if (retval < 0)
	//{
	//	perror("setsockopt() failed");
	//	close(listen_sd);
	//	exit(-1);
	//}

	if (bind(Listen, pResults->ai_addr, pResults->ai_addrlen) == -1) {
		closesocket(Listen);
		perror("server: bind");
	}

	if (pResults == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}

	if (listen(Listen, 10) == -1) {
		perror("listen");
		exit(1);
	}

	freeaddrinfo(pResults);

	printf("Start server\n");
	char m_connect[] = "Connect to server";

	for (;; Sleep(75))
	{
		fd_set afds, rfds;
		FD_ZERO(&afds);
		FD_SET(0, &afds);

		FD_ZERO(&rfds);
		FD_SET(Listen, &rfds);

		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 1;

		pollfd pfds[200];
		int nfds = 1;

		memset(pfds, 0, sizeof(pfds));

		while (!end_server)
		{
			pfds[0].fd = Listen;
			pfds[0].events = POLLIN;

			timeout.tv_sec = 0;
			timeout.tv_usec = 1;

			retval = poll(pfds, nfds, timeout.tv_usec);

			if (retval < 0)
			{
				//perror("Poll error");
				break;
			}
			if (retval == 0)
			{
				perror("Timeout poll error");
				break;
			}
			int current_size = nfds;
			for (int i = 0; i < current_size; i++)
			{
				printf("Enter in main loop");

				if (pfds[i].revents == 0)
				{
					continue;
				}
				if (pfds[i].revents != POLLIN)
				{
					printf("  Error! revents = %d\n", pfds[i].revents);
					end_server = true;
					break;
				}
				if (pfds[i].fd == Listen)
				{
					printf("  Listening socket is readable\n");

					do
					{
						Connect = accept(Listen, NULL, NULL);
						if (Connect <= 0)
						{
							if (errno != EWOULDBLOCK)
							{
								perror("  accept() failed");
								end_server = true;
							}
							if (Connect == INVALID_SOCKET)
							{
								wsaError = GetLastError();

								if (wsaError != WSAEWOULDBLOCK)
								{
									CheckSockError(wsaError);
									end_server = true;
								}
							}
							break;
						}

						printf("  New incoming connection - %d\n", Connect);
						pfds[nfds].fd = Connect;
						pfds[nfds].events = POLLIN;
						nfds++;
					} while (Connect > 0);
				}
				else
				{
					printf("   Descriptor %d is readable\n", pfds[i].fd);
					close_conn = false;
					do
					{
						int retRecv = recv(pfds[i].fd, buffer, sizeof(buffer), NULL);
						if (retRecv < 0)
						{
							wsaError = GetLastError();

							if (errno != EWOULDBLOCK)
							{
								perror("   recv() failed");
								close_conn = true;
							}

							if (wsaError != WSAEWOULDBLOCK)
							{
								perror("   recv() failed");
								close_conn = true;
							}
							break;
						}
						else if (retval == 0)
						{
							close_conn = true;
							break;
						}

						int len = retval;
						printf("  &d bytes to recieved\n", len);

						retval = send(pfds[i].fd, buffer, len, 0);
						if (retval < 0)
						{
							perror("   sebd() failed");
							close_conn = true;
							break;
						}
					} while (1);

					if (close_conn)
					{
						printf("  Connection closed\n");
						closesocket(pfds[i].fd);
						pfds[i].fd = -1;
						compress_array = true;
					}
				}
			}

			if(compress_array)
			{
				for(int i = 0;i < nfds; i++)
				{
					if(pfds[i].fd == -1)
					{
						for (int j = i; j < i; j++)
						{
							pfds[j].fd = pfds[j + 1].fd;
						}
						i--;
						nfds--;
					}
				}
			}
		}
		for(int i = 0; i < nfds; i++)
		{
			if(pfds[i].fd >= 0)
			{
				closesocket(pfds[i].fd);
			}
		}
		WSACleanup();
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

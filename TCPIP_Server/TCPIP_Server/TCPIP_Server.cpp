// TCPIP_Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once

# pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <algorithm>

#include <thread>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <locale>
#include "../../ProtocolAPI/ProtocolAPI/Protocol.h"
#include <unordered_map>
#include <map>

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
#define MAX_LENGTH 1024
#define HEADER_START 0xFFFFFFFF
#define MESSAGE_START_INDEX 7
#define RESPONSE_OK_LENGTH 7
//
////TODO check!
//#if defined(WIN32) && !defined(UNIX)
///* Do windows stuff */
//#elif defined(UNIX) && !defined(WIN32)
////	#define SOCKET unsigned int *
//#else
///* Error, both can't be defined or undefined same time */
//#endif

int ConnectSock;
std::vector<int> ConnectionSockets;
std::vector<int>::iterator it;
std::vector<int>::iterator anotherIt;

//std::vector<std::string> vecClientNames;
std::unordered_map<std::string, int>::iterator itNameMap;
std::unordered_map<std::string, int> mapClientNames;

std::string strNameClient;

int ListenSock;

int ClientCount = 0;

int wsaError = 0;

void CheckSockError(int wsaError);
static inline char* rtrim(char* buffer);
void printWsaError();

int main()
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

	struct addrinfo hints, *p;
	struct addrinfo *pResults;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = PF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//TODO why res?
	int res;
	if ((res = getaddrinfo(NULL, "7700", &hints, &pResults)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(res));
		return 1;
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
		return 2;
	}

	if (listen(ListenSock, 10) == -1) {
		printf("listen");
		exit(1);
	}

	printf("Start server\n");
	char m_connect[] = "ConnectSock to server \n";

	freeaddrinfo(pResults);

	for (;; Sleep(75))
	{
		fd_set exeptfds, rfds;
		//FD_ZERO(&exeptfds);
		//FD_SET(0, &exeptfds);

		FD_ZERO(&rfds);
		FD_SET(ListenSock, &rfds);

		int maxFD = ListenSock;

		for (it = ConnectionSockets.begin(); it != ConnectionSockets.end(); ++it)
		{
			//TODO check s is ref?
			//TODO maybe add in vector at this place
			int s = *it;
			FD_SET(s, &rfds);
			maxFD = max(maxFD, s) + 1;
		}

		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 500;

		int count = select(maxFD, &rfds, NULL, NULL, &timeout);

		struct sockaddr_storage their_addr;

		socklen_t addr_size = sizeof their_addr;

		if (count)
		{
			std::vector<std::vector<int>::iterator> clientsToDelete;
			for (it = ConnectionSockets.begin(); it != ConnectionSockets.end() && count; it++)
			{
				if (FD_ISSET(*it, &rfds))
				{
					--count;

					//memset(buffer, 0, sizeof buffer);

					//TODO how to know message size?
					int nRecievedBytes = recv(*it, buffer, MAX_LENGTH - 1, 0);

					if (nRecievedBytes == 0)
					{
						printf("server: client close recieve \n");
						//TODO i think buffer was read until not reach '\0'
						//TODO how to print without trash?
						//std::cout << "Message: " << std::string(buffer, res) << std::endl;
						clientsToDelete.push_back(it);
					}
					else if (nRecievedBytes == -1)
					{
						std::cout << "server: recieve error" << std::endl;
						printWsaError();
						clientsToDelete.push_back(it);
					}
					else
					{
						//int* test = (int*)buffer;
						if(*(int*)buffer == HEADER_START)
						{
							int nNameLen = 0;

							switch (buffer[5])
							{
							case ChatLib::eSendName:

								nNameLen = buffer[6];

								strNameClient.erase();

								for(int i = MESSAGE_START_INDEX; i < nNameLen + MESSAGE_START_INDEX; ++i)
								{
									strNameClient.push_back(buffer[i]);
								}

								if(mapClientNames.find(strNameClient) != mapClientNames.end())
								{
									//TODO sendErrorResponse()
									std::string msg = HEADER_START + ChatLib::eError + strNameClient.length() + "Name " + strNameClient + " already used";
									int nMustSend = msg.length();
									int sended = 0;
									do
									{
										if((sended = send(*it, msg.c_str(), nMustSend, NULL) == -1))
										{
											//TODO check error
											break;
										}
										nMustSend -= sended;
									}
									while (nMustSend);

									timeval timer;
									timer.tv_sec = 0;
									timer.tv_usec = 500;

									//TODO send response oK

									char buffResponse[RESPONSE_OK_LENGTH];
									int recieved = 0;
									do
									{
										recieved = recv(*it, buffResponse, RESPONSE_OK_LENGTH, NULL);
										if(recieved == 0)
										{
											//TODO check error or close conn
										}
										else if(recieved == -1)
										{
											//TODO checkerror
										}else
										{
											//TODO recieve response
										}
									}
									while (recieved != RESPONSE_OK_LENGTH);
								}
								else
								{
									mapClientNames.insert(std::make_pair(strNameClient, *it));
									char tempBuff[RESPONSE_OK_LENGTH];

									int nByte = 0;
									tempBuff[nByte++] = 0xFF; //
									tempBuff[nByte++] = 0xFF; // Packet start magic number
									tempBuff[nByte++] = 0xFF; //
									tempBuff[nByte++] = 0xFF; //
									tempBuff[nByte++] = ChatLib::eResponce;

									tempBuff[nByte++] = ChatLib::eOk;
									tempBuff[nByte++] = 0;

									int nMustSend = RESPONSE_OK_LENGTH;
									int sended = 0;
									do
									{
										if ((sended = send(*it, tempBuff, nMustSend, NULL) == -1))
										{
											//TODO check error
											break;
										}
										nMustSend -= sended;
									} while (nMustSend);
								}
								
								break;
							case ChatLib::eSendMessage:

							for (itNameMap = mapClientNames.begin();itNameMap != mapClientNames.end(); itNameMap++ )
							{

								if(itNameMap->second == *it)
								{
									int nMessageLength = buffer[6];

									std::string msg;
									for (int i = MESSAGE_START_INDEX; i < nMessageLength + MESSAGE_START_INDEX; ++i)
									{
										msg.push_back(buffer[i]);
									}

									std::cout << itNameMap->first + ": " + msg << std::endl;
									//TODO send respoce;

									char tempBuff[RESPONSE_OK_LENGTH];

									int nByte = 0;
									tempBuff[nByte++] = 0xFF; //
									tempBuff[nByte++] = 0xFF; // Packet start magic number
									tempBuff[nByte++] = 0xFF; //
									tempBuff[nByte++] = 0xFF; //
									tempBuff[nByte++] = ChatLib::eResponce;

									tempBuff[nByte++] = ChatLib::eOk;
									tempBuff[nByte++] = 0;

									int nMustSend = RESPONSE_OK_LENGTH;
									int sended = 0;
									do
									{
										if ((sended = send(*it, tempBuff, nMustSend, NULL) == -1))
										{
											//TODO check error
											break;
										}
										nMustSend -= sended;
									} while (nMustSend);

								}
								else
								{
									std::cout << "Error: Dont know this user name"<< std::endl;
								}
							}
								break;
							}
						}

						//TODO create method
						//char msgInfo[4];
						//int j = 0;
						//int msgLength = 0;
						//int msgStartIndex = 0;
						//for (int i = 0; i < nRecievedBytes + 1; ++i)
						//{
						//	if (buffer[i] == '|')
						//	{
						//		while (buffer[i + 1] != '|')
						//		{
						//			msgInfo[j++] = buffer[i + 1];
						//			i++;
						//		}

						//		msgLength = atoi(msgInfo);
						//		if(msgLength / 1000  > 0)
						//		{
						//			msgLength -= 6;
						//		}else if(msgLength / 100 > 0)
						//		{
						//			msgLength -= 5;
						//		}else if (msgLength / 10 > 0)
						//		{
						//			msgLength -= 4;
						//		}else
						//		{
						//			msgLength -= 3;
						//		}

						//		msgStartIndex = i + 2;
						//		break;
						//	}
						//}

						////TODO why?
						//char msgArr [MAX_LENGTH];
						////char *msgArr = new char(msgLength);
						////char *msgArr = (char *)malloc(msgLength * sizeof(char));
						////std::shared_ptr<char> msgArr2(new char(msgLength));
						//for(int i = 0; i != msgLength; ++i)
						//{
						//	msgArr[i] = buffer[msgStartIndex + i];
						//}


						//std::string message;
						//TODO create method

						std::cout << "Byte recieved: " << nRecievedBytes << std::endl;

						for (anotherIt = ConnectionSockets.begin(); anotherIt != ConnectionSockets.end(); anotherIt++)
						{
							if (true)//(anotherIt != it)
							{
								int mustSend = nRecievedBytes;
								int sended = 0;

								char msg[] = "This is test message";

								do
								{
									//TODO error num bytes for send
									sended = send(*anotherIt, msg, nRecievedBytes, NULL);
									mustSend -= sended;

									if (sended == -1)
									{
										break;
										//TODO check error
									}
								} while (mustSend != 0);
							}
						}

						//std::cout << "Message: " << message << std::endl;
					}
				}
			}
			for (auto client_to_delete : clientsToDelete)
			{
				ConnectionSockets.erase(client_to_delete);
			}

			if (FD_ISSET(ListenSock, &rfds))
			{
				int result = accept(ListenSock, (struct sockaddr*)&their_addr, &addr_size);
				if (result <= 0)
				{
					printf("server: accept error \n");
				}
				else
				{
					printf("Client was connected");
					int s = result;
					ConnectionSockets.push_back(s);
				}
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

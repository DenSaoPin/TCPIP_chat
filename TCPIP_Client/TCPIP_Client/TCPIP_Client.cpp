// TCPIP_Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma comment (lib, "Ws2_32.lib")

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <string>
//#include <sys/types.h>
//#include <sys/socket.h>

#define MAX_PORT_DIGIT 5

int main()
{
	setlocale(LC_ALL, "Russian");

	char Name[1024];

	std::cout << "Enter your name:\n";
	std::cin >> Name;

	char ServerIP[INET_ADDRSTRLEN];
	memset(ServerIP, ' ', sizeof ServerIP);
	//MAX_PORT_NUM = 65535
	char ServerPort[MAX_PORT_DIGIT];
	memset(ServerPort, ' ', sizeof ServerPort);


	std::cout << "Enter Server IP or press Enter for use localhost" << std::endl;

	//TODO Do we have best clean \n method?
	std::cin.ignore();

	//TODO check 
	std::string str;
	std::getline(std::cin, str);

	//TODO check
	if (str.length() == 0)
	{
		strncpy(ServerIP, std::string("127.0.0.1").c_str(), 10);
	}
	else if (str.length() > INET_ADDRSTRLEN)
	{
		printf("So much numbers for IPv4");
	}
	else
	{
		strncpy(ServerIP, str.c_str(), str.length() + 1);
	}

	std::cout << "Enter Server Port or press Enter for use default 7700" << std::endl;
	str.clear();
	std::getline(std::cin, str);

	//TODO check
	if (str.length() == 0)
	{
		strncpy(ServerPort, std::string("7700").c_str(), 10);
	}
	else if (str.length() > MAX_PORT_DIGIT)
	{
		printf("So much numbers for IPv4");
	}
	else
	{
		strncpy(ServerPort, str.c_str(), str.length() + 1);
	}

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		std::cout << "Could not initialize winsock" << std::endl;
	}

	struct addrinfo hints;
	struct addrinfo *servinfo;

	memset(&hints, 0, sizeof hints);

	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	//hints.ai_flags = AI_PASSIVE;


	//TODO check errors
	if (int status = getaddrinfo(ServerIP, ServerPort, &hints, &servinfo) != 0)
	{
		std::cout << "getaddrinfo error: " << gai_strerror(status) << std::endl;
	}


	int sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	//TODO maybe need bind

	//connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
	if (int status = connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) != 0)
	{
		std::cout << "connect error: " << gai_strerror(status) << std::endl;
	}

	//bind(s, hints, hints.ai_addrlen)
	char buff[1024];

	sprintf(buff, "%s: Test Message", Name);

	//fd_set rfds;
	//select()
	//FD_SET()
	while (true)
	{
		int length = strlen(buff);
		int sended = 0;
		do
		{
			sended = send(sockfd, buff, length, NULL);
			length -= sended;
			if(sended = -1)
			{
				//TODO check error WIN \ UNIX
			}
		} while (length > 0);

	}

	closesocket(sockfd);
	std::cout << "Hello World!\n";

	return 0;
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

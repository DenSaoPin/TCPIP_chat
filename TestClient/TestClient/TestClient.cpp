# pragma comment(lib, "Ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <tchar.h>
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>


#define PORT "7700" // Порт, к которому подключается клиент

#define MAXDATASIZE 100 // максимальное число байт, принимаемых за один раз

// получение структуры sockaddr, IPv4 или IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == 2) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[65];

	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);

	int wsaInitError = WSAStartup(wVersion, &wsaData);

	if (wsaInitError != 0)
	{
		std::cout << "Could not initialize winsock" << std::endl;
	}

	if (argc != 2) {
		fprintf(stderr, "usage: client hostname\n");
		exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = 0;
	hints.ai_socktype = 1;

	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0)
	{

		fprintf(stderr, "getaddrinfo: %ls\n", gai_strerror(rv));
		return 1;
	}

	// Проходим через все результаты и соединяемся к первому возможному
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) 
		{
			perror("client: socket\n");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			closesocket(sockfd);
			perror("client: connect\n");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
		s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); 

	while (1)
	{
		std::cout << "Your message: \n";

		std::cin >> buf;

		send(sockfd, buf, strlen(buf + 1), 0);

		if ((numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0)) == -1)
		{
			perror("recv");
			exit(1);
		}
		buf[numbytes] = 0;
		printf("client: received '%s'\n", buf);

	}


	closesocket(sockfd);

	return 0;
}
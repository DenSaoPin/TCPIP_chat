#define MAX_CLIENTS 10
#define MAX_LENGTH 1024
#define HEADER_START 0xFFFFFFFF

#ifdef _WIN32
    #define SLEEP Sleep
#elif  __GNUC__
    #include <unistd.h>
    #define SLEEP usleep
#else
#define SLEEP usleep
#endif

#include <string>
#include "ServerClient.h"
int wsaError = 0;

void CheckSockError(int wsaError);
static inline char* rtrim(char* buffer);

int main()
{
	std::string strNameClient;
	
	TCPServer server("0.0.0.0", "7700");

    for (;; SLEEP(75))
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

#if defined _WIN32
    void TCPServer::printWsaError()
{
	switch (WSAGetLastError())
	{
	case WSANOTINITIALISED: printf("WSANOTINITIALISED: A successful WSAStartup call must occur before using this function.\n");
		//WSACleanup();
		break;

	case WSAENETDOWN: printf("WSAENETDOWN: The network subsystem has failed.\n");
		//WSACleanup();
		break;

	case WSAEFAULT: printf("WSAEFAULT: The buf parameter is not completely contained in a valid part of the user address space.\n");
		//WSACleanup();
		break;

	case WSAENOTCONN: printf("WSAENOTCONN: The socket is not connected.\n");
		//WSACleanup();
		break;

	case WSAEINTR: printf("WSAEINTR: The (blocking) call was canceled through WSACancelBlockingCall.\n");
		//WSACleanup();
		break;

	case WSAEINPROGRESS: printf("WSAEINPROGRESS: A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.\n");
		//WSACleanup();
		break;

	case WSAENETRESET: printf("WSAENETRESET: For a connection-oriented socket, this error indicates that the connection has been broken due to keep-alive activity that detected a failure while the operation was in progress. For a datagram socket, this error indicates that the time to live has expired.\n");
		//WSACleanup();
		break;

	case WSAENOTSOCK: printf("WSAENOTSOCK: The descriptor is not a socket.\n");
		//WSACleanup();
		break;

	case WSAEOPNOTSUPP: printf("WSAEOPNOTSUPP: MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations.\n");
		//WSACleanup();
		break;

	case WSAESHUTDOWN: printf("WSAESHUTDOWN: The socket has been shut down; it is not possible to receive on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH.\n");
		//WSACleanup();
		break;

	case WSAEWOULDBLOCK: printf("WSAEWOULDBLOCK: The socket is marked as nonblocking and the receive operation would block.\n");
		//WSACleanup();
		break;

	case WSAEMSGSIZE: printf("WSAEMSGSIZE: The message was too large to fit into the specified buffer and was truncated.\n");
		//WSACleanup();
		break;

	case WSAEINVAL: printf("WSAEINVAL: The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative.\n");
		//WSACleanup();
		break;

	case WSAECONNABORTED: printf("WSAECONNABORTED: The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.\n");
		//WSACleanup();
		break;

	case WSAETIMEDOUT: printf("WSAETIMEDOUT: The connection has been dropped because of a network failure or because the peer system failed to respond.\n");
		//WSACleanup();
		break;

	case WSAECONNRESET: printf("WSAECONNRESET: The virtual circuit was reset by the remote side executing a hard or abortive close. The application should close the socket as it is no longer usable. On a UDP-datagram socket, this error would indicate that a previous send operation resulted in an ICMP \"Port Unreachable\" message.\n");
		//WSACleanup();
		break;
	}
}
#endif
void TCPServer::printLinuxError()
{
    printf("%s", strerror(errno));
}

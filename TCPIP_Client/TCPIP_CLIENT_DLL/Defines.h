#pragma once

#ifdef _WIN32

#include <WinSock2.h>
#define PrintErrors printWsaError()
#define CROSS_SOCKET SOCKET
#define CROSS_SIZE int

#else

#ifdef __GNUC__

#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
// types.h need for backward compatibility with BSD
#include <sys/types.h>
#define PrintErrors printLinuxError()
#define CROSS_SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define CROSS_SIZE socklen_t
#define CROSS_SOCKCLOSE

#endif
#endif

#if defined _WIN32
inline void printWsaError()
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

inline void printLinuxError()
{
    printf("%s", strerror(errno));
    /*
    switch (errno())
    {

    case EINTR: printf("The function specified was interrupted by a signal."); break;
    case EBADF: printf("Bad socket. The socket might have been corrupted."); break;
    case EAGAIN: printf("Resource temporarily unavailable."); break;
    case EFAULT: printf("Bad address.On connect, bad address.On receive, the data was directed to be received into a nonexistent or protected art of the process address space. The buffer is not valid."); break;
    case EBUSY: printf("Resource busy."); break;
    case EINVAL: printf("Invalid argument that is passed to the specified function or the socket is closed. The EINVAL error can be returned if here was a memory overwrite, or buffer overflow problem."); break;
    case ENFILE: printf("Too many open files in the system."); break;
    case EMFILE: printf("The per-process file descriptor table is full. Number of file descriptors/sockets for the process is exceeded."); break;
    case ENOSPC: printf("No space is left on a device or system table."); break;
    case EPIPE: printf("Broken pipe."); break;
    case EWOULDBLOCK: printf("On the connect function, the range that is allocated for TCP/UDP ephemeral ports is exhausted. (Some operating systems return the same error as EAGAIN.)"); break;
    case ENOTSOCK: printf("Socket operation on a non-socket."); break;
    case ENOPROTOOPT: printf("Option is unknown."); break;
    case EADDRINUSE: printf("The specified address is already in use. Perhaps the previous process that established the connection was terminated abnormally or was not cleaned up properly."); break;
    case EADDRNOTAVAIL: printf("The specified host name or IP address is not available from the local system."); break;
    case ENETDOWN: printf("The network is down."); break;
    case ENETUNREACH: printf("No route to the network or host is available."); break;
    case ENETRESET: printf("The network dropped the connection on reset."); break;
    case ECONNRESET: printf("Connection was reset by the partner."); break;
    case ENOBUFS: printf("Insufficient memory or resources were available in the system to complete the call."); break;
    case EISCONN: printf("The socket is already connected."); break;
    case ENOTCONN: printf("The socket is not connected."); break;
    case ETIMEDOUT: printf("The connection was timed out."); break;
    case ECONNREFUSED: printf("The connection was refused. If you are trying to connect to the database, check that the database manager and TCP/IP protocol support at the server were started successfully.If you specified the SOCKS protocol support, you must also ensure that the TCP/IP protocol support at the SOCKS server was started successfully."); break;
    case EHOSTDOWN: printf("The host is down."); break;
    case EHOSTUNREACH: printf("No route to the host is available."); break;
    }
               */
}

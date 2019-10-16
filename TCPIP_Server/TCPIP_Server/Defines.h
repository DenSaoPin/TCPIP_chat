#pragma once

#ifdef _WIN32

    #include <WinSock2.h>
    #define PrintErrors printWsaError()
    #define CROSS_SOCKET SOCKET
    #define CROSS_SIZE int

#else

    #ifdef __GNUC__

        #define PrintErrors printLinuxError()

        #include <string.h>
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <netdb.h>
        #include <errno.h>
        #include <fcntl.h>
        #include <unistd.h>
        // types.h need for backward compatibility with BSD
        #include <sys/types.h>

        #define CROSS_SOCKET int
        #define INVALID_SOCKET -1
        #define SOCKET_ERROR -1
        #define CROSS_SIZE socklen_t
        #define CROSS_SOCKCLOSE
    #endif
#endif


#pragma once

#if defined _WIN32
#include <WinSock2.h>
#define PrintErrors printWsaError()
#define CROSS_SOCKET SOCKET
#else
#define PrintErrors printLinuxError()
#endif

inline void printWsaError()
{
	
}
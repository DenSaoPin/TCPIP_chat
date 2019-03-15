#pragma once
#include <string>
#include <vector>
#include "Response.h"
#include "Enums.h"

#if defined _WIN32
#include <WinSock2.h>
#define PrintErrors printWsaError()
#define CROSS_SOCKET SOCKET
#else
#define PrintErrors printLinuxError()
#endif

typedef std::vector<CROSS_SOCKET> SocketsVector;
typedef SocketsVector::iterator SocketsVectorIterator;

namespace ChatLib
{
	class Message;

	namespace Protocol
	{
		class ConnectionLostException : public std::exception
		{
		public: ConnectionLostException(const char * text) : exception(text) {}
		};
		class ConnectionClosedException : public std::exception
		{
		public: ConnectionClosedException(const char * text) : exception(text) {}
		};

		Response TrySendMessage(BaseMessage*, const CROSS_SOCKET& socket);

		RawBytes RecieveMessageAndReply(const CROSS_SOCKET& socket);

		RawBytes RecieveMessage(const CROSS_SOCKET& socket);

		void SendMessagee(BaseMessage* , const CROSS_SOCKET& socket);

		void SendResponse(ResponseStatus status, const CROSS_SOCKET& socket);

		const bool IsLegalPackage(const char* buff);

		//const MessageType GetMessageType(const char* buff);

		const int IncomingMessageNum(const SocketsVector &sockVec);

		void printWsaError();

		void printLinuxError();
	};
}
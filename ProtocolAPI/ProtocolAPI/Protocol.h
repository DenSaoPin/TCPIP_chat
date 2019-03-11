#pragma once
#include <string>
#include "Enums.h"
#include <vector>

#if defined _WIN32
#include <WinSock2.h>
#define PrintErrors printWsaError()
#define CROSS_SOCKET SOCKET
#else
#define PrintErrors printLinuxError()
#endif

typedef std::vector<CROSS_SOCKET> SocketsVector;
typedef SocketsVector::iterator SocketsVectorIterator;

#define MAX_PACKAGE_LENGTH 255
#define HEADER_SIZE 6
#define HEADER_START 0xFFFFFFFF
#define RESPONSE_OK_LENGTH 5
#define MESSAGE_LENGTH_INDEX 5
#define MESSAGE_TYPE_INDEX 4

#define MESSAGE_START_TEXT_INDEX 6



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
		const MessageType TrySendMessage(const Message& message, const CROSS_SOCKET& socket);

		const Message TryRecieveMessage(const CROSS_SOCKET& socket);

		const Message RecieveMessage(const CROSS_SOCKET& socket);

		void SendMessagee(const Message& message, const CROSS_SOCKET& socket);

		void SendResponse(const MessageType& responceVal, const CROSS_SOCKET& socket);

		const bool IsLegalPackage(const char* buff);

		const MessageType GetMessageType(const char* buff);

		const int IncomingMessageNum(const SocketsVector &sockVec);

		void printWsaError();

		void printLinuxError();
	};
}
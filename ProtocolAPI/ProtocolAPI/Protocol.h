#pragma once
#include <string>
#include "Enums.h"
//#include "Message.h"
#include <vector>

#if defined _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#define CROSS_SOCKET SOCKET
#else
#endif

typedef std::vector<CROSS_SOCKET> SocketsVector;
typedef SocketsVector::iterator SocketsVectorIterator;

#define MAX_PACKAGE_LENGTH 255
#define HEADER_SIZE 6
#define HEADER_START 0xFFFFFFFF
#define RESPONSE_OK_LENGTH 5
#define MESSAGE_LENGTH_INDEX 5
#define MESSAGE_TYPE_INDEX 4
//#define CONTENT_TYPE_INDEX 5

#define MESSAGE_START_TEXT_INDEX 6



namespace ChatLib
{
	class Message;

	namespace Protocol
	{
		const MessageType TrySendMessage(const Message& message, const CROSS_SOCKET& socket);

		const Message TryRecieveMessage(const CROSS_SOCKET& socket);

		const Message RecieveMessage(const CROSS_SOCKET& socket);

		void SendMessagee(const Message& message, const CROSS_SOCKET& socket);

		//MessageType RecieveAnswer(int socket);

		void SendResponse(const MessageType& responceVal, const CROSS_SOCKET& socket); //, std::string strMessage);

		const bool IsLegalPackage(const char* buff);

		//MessageType CheckResponseStatus(char* buff);

		const MessageType GetMessageType(const char* buff);

		const int IncomingMessageNum(const SocketsVector &sockVec);
	};
}
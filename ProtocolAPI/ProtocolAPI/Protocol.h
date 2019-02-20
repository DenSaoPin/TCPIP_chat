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
		MessageType TrySendMessage(Message message, CROSS_SOCKET socket);

		Message TryRecieveMessage(CROSS_SOCKET socket);

		Message RecieveMessage(CROSS_SOCKET socket);

		void SendMessagee(Message message, CROSS_SOCKET socket);

		//MessageType RecieveAnswer(int socket);

		void SendResponse(MessageType responceVal, CROSS_SOCKET socket);//, std::string strMessage);

		bool IsLegalPackage(char* buff);

		//MessageType CheckResponseStatus(char* buff);

		MessageType GetMessageType(char* buff);

		int IncomingMessageNum(SocketsVector &sockVec);

		//int IncomingMessageNum(int &socket);

		void f1();
	};
}
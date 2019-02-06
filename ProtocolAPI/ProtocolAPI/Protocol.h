#pragma once
#include <string>
#include "Enums.h"
//#include "Message.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>

#define MAX_PACKAGE_LENGTH 255
#define HEADER_SIZE 6
#define HEADER_START 0xFFFFFFFF
#define RESPONSE_OK_LENGTH 5
#define MESSAGE_LENGTH_INDEX 5
#define MESSAGE_TYPE_INDEX 4
//#define CONTENT_TYPE_INDEX 5

#define MESSAGE_START_INDEX 6



namespace ChatLib
{
	class Message;

	namespace Protocol
	{
		MessageType TrySendMessage(Message message, int socket);

		Message TryRecieveMessage(int socket);

		Message RecieveMessage(int socket);

		void SendMessagee(Message message, int socket);

		MessageType RecieveAnswer(int socket);

		void SendResponse(MessageType responceVal, int socket, std::string* pstrMessage = nullptr);

		bool IsLegalPackage(char* buff);

		//MessageType CheckResponseStatus(char* buff);

		MessageType GetMessageType(char* buff);

		int IncomingMessageNum(std::vector<int> &sockVec);

		//int IncomingMessageNum(int &socket);

		void f1();
	};
}
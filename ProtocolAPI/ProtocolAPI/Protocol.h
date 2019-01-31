#pragma once
#include <string>
#include "Enums.h"
#include "Message.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

#define MAX_PACKAGE_LENGTH 255
#define HEADER_SIZE 7
#define HEADER_START 0xFFFFFFFF
#define RESPONSE_OK_LENGTH 6
#define MESSAGE_LENGTH_INDEX 6
#define MESSAGE_TYPE_INDEX 4
#define CONTENT_TYPE_INDEX 5

namespace ChatLib
{
	namespace Protocol
	{
		Response TrySendMessage(Message message, int socket);

		Message TryRecieveMessage(int socket);

		Message RecieveMessageFromServer(int socket);

		void SendMessagee(Message message, int socket);

		Response RecieveAnswer(int socket);

		void SendResponse(Response responceVal, int socket, std::string* pstrMessage = nullptr);

		bool IsLegalPackage(char* buff);

		Response CheckResponseStatus(char* buff);

		MessageType GetMessageType(char* buff);

		void f1();
	};
}
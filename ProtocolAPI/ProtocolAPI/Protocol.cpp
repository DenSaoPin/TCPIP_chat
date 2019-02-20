#include "stdafx.h"
#include "Protocol.h"
#include "Message.h"
#include <vector>

namespace ChatLib
{
	const MessageType Protocol::TrySendMessage(const Message& message,const CROSS_SOCKET& socket)
	{
		//TODO need to know sent message or not
		SendMessagee(message, socket);

		Message response = RecieveMessage(socket);

		return response.GetType();
	}

	const Message Protocol::TryRecieveMessage(const CROSS_SOCKET& socket)
	{
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(socket, &rfds);
		int maxFD = (int)socket + 1;

		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 500;
		Message message(eInvalid);

		int result = select(maxFD, &rfds, NULL, NULL, &timeout);
		if (result < 0)
		{
			//TODO UNIX check error
		}
		else if (result == 0)
		{ 

			//TODO win check error and timeout
		}
		else
		{
			if (FD_ISSET(socket, &rfds))
			{
				message = RecieveMessage(socket);

				if(message.GetType() != eInvalid)
					SendResponse(eResponseOk, socket);
			}
		}
		return message;
	}

	 const Message Protocol::RecieveMessage(const CROSS_SOCKET& socket)
	{
		//TODO check recieved num
		char buff[MAX_PACKAGE_LENGTH];
		int recived = recv(socket, buff, MAX_PACKAGE_LENGTH, NULL);
		printf("RecieveMessage %d bytes: ", recived);
		for (int i = 0; i < recived; i++)
		{
			printf("%02X ", buff[i]);
		}
		printf("\n");
		if (recived == 0)
		{
			//TODO error check Win and Timeout
		}
		else if (recived == -1)
		{
			//TODO check Unix error
		}
		else
		{
			std::string str = "";

			//TODO refactor It must be inside message
			//TODO maybe lost data after recieve
			//buff[4] = 4;

			MessageType type = GetMessageType(buff);

			printf("Recieved message type equal %d \n", type);
    		//Message message(eInvalid);

			Message message (type, buff);

			printf("Recieved message equal %s \n", message.GetText());

			//if(*(int*)buff == HEADER_START)
			//{
			//	return  Message (type, buff);
			//}
			//else
			//{
			//	throw new std::exception(" Not filled package type or package is trash \n");
			//}
				
			return message;
			//if (IsLegalPackage(buff))
			//{
			//	std::string str;
			//	str.append(buff, HEADER_SIZE + buff[6]);
			//	Message message(str);
			//	return message;
			//}
			//else
			//{
			//	throw new std::exception("It isn`t message");
			//}
		}
		return Message(eInvalid);
	}

	void Protocol::SendMessagee(const Message& message, const CROSS_SOCKET& socket)
	{
		//TODO refactor this
		//message.

		char pBuffer[255];

		int sended = 0;
		int length = message.WriteBytes(pBuffer);
		printf("Sending %d bytes: ", length);
		for(int i = 0; i < length; i++)
		{
			printf("%02X ", pBuffer[i]);
		}
		printf("...\n");
		do
		{
			//TODO check with length = 1
			//TODO check & + [] + string
			sended = send(socket, pBuffer, length, NULL);
			printf("Sended %d\n", sended);
			length -= sended;
			if (sended == -1)
			{
				break;
				//TODO check error WIN \ UNIX
			}
		} while (length > 0);
	}

	//MessageType Protocol::RecieveAnswer(int socket)
	//{
	//	char recvBuffer[RESPONSE_OK_LENGTH];

	//	int recieved = 0;
	//	MessageType response = eInvalid;
	//	do
	//	{
	//		recieved = recv(socket, recvBuffer, RESPONSE_OK_LENGTH, NULL);
	//		printf("RecieveAnswer %d bytes: ", recieved);
	//		for (int i = 0; i < recieved; i++)
	//		{
	//			printf("%02X ", recvBuffer[i]);
	//		}
	//		printf("\n");
	//		if (recieved == 0)
	//		{
	//			//TODO check error or close conn
	//		}
	//		else if (recieved == -1)
	//		{
	//			//TODO checkerror
	//		}
	//		else
	//		{
	//			return response = GetMessageType(recvBuffer);
	//		}
	//		//TODO maybe while responce != oK
	//	}
	//	//while (recieved < RESPONSE_OK_LENGTH);
	//	//TODO check 
	//	while (true);
	//}

	void Protocol::SendResponse(const MessageType& messageType, const CROSS_SOCKET& socket)    //, std::string strMessage)
	{
		Message message(messageType);                                   //, strMessage);
		SendMessagee(message, socket);
	}

	const bool Protocol::IsLegalPackage(const char* buff)
	{
		return false;
	}

	//MessageType Protocol::CheckResponseStatus(char* buff)
	//{
	//	if (*(int *)buff == HEADER_START)
	//	{
	//		//TODO check
	//		if ((int)buff[4] == eResponce)
	//		{
	//			if ((int)buff[5] == eOk)
	//				return eOk;
	//			else if ((int)buff[5] == eError)
	//				return eError;
	//			else
	//			{
	//				//TODO
	//				//throw new std::exception(" Wrong Response \n");
	//				//std::cout << " Wrong Response \n";
	//			}
	//		}
	//	}
	//	else
	//	{
	//		//TODO exception
	//	}
	//}

	const MessageType Protocol::GetMessageType(const char* buff)
	{
		if (*((int*)buff) == HEADER_START)
		{
			char messageType = buff[MESSAGE_TYPE_INDEX];

			if (messageType == eNameRequest ||
				messageType == eMessageRequest ||
				messageType == eResponseOk ||
				messageType == eResponceError)
				return (MessageType)messageType;

			throw std::exception(" Not filled package type or package is trash \n");
		}
		throw std::exception(" It isn`t message \n");
	}

	int IncomingMessageNum(SocketsVector &socketVec)
	{
		fd_set rfds;
		FD_ZERO(&rfds);

		SocketsVectorIterator it;

		int maxNumFD = 0;
		for (it = socketVec.begin(); it != socketVec.end(); it++)
		{
			FD_SET(*it, &rfds);
			maxNumFD = max(maxNumFD, (int)*it);
		}
		maxNumFD++;

		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 500;

		//Message message;

		int result = select(maxNumFD, &rfds, NULL, NULL, &timeout);
		return result;
	}
	int IncomingMessageNum(int &sockfd)
	{
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(sockfd, &rfds);
		int maxNumFD = sockfd + 1;

		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 500;

		int result = select(maxNumFD, &rfds, NULL, NULL, &timeout);

		//TODO error check
		return result;
	}

}

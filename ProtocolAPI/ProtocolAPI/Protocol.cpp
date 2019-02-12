#include "stdafx.h"
#include "Protocol.h"
#include "Message.h"
#include <iostream>
#include <vector>

namespace ChatLib
{
	MessageType Protocol::TrySendMessage(Message message, int socket)
	{
		//TODO need to know sent message or not
		SendMessagee(message, socket);

		MessageType messageType = RecieveAnswer(socket);

		return messageType;
	}

	Message Protocol::TryRecieveMessage(int socket)
	{
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(socket, &rfds);
		int maxNumFD = socket;

		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 500;

		Message message;

		int result = select(maxNumFD, &rfds, NULL, NULL, &timeout);
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

				SendResponse(eResponseOk, socket);
			}
		}
		return message;
	}

	Message Protocol::RecieveMessage(int socket)
	{
		//TODO check recieved num
		char buff[MAX_PACKAGE_LENGTH];
		int recived = recv(socket, buff, sizeof buff, NULL);
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

			MessageType type = GetMessageType(buff);

			Message message(type, buff);
		
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
	}

	void Protocol::SendMessagee(Message message, int socket)
	{
		//TODO refactor this
		//message.
		std::string package = message.GetHeader() + message.GetText();
		int sended = 0;
		int length = package.length();
		do
		{
			//TODO check with length = 1
			//TODO check & + [] + string
			sended = send(socket, &(package.c_str()[package.length() - length]), length,
				NULL);
			length -= sended;
			if (sended == -1)
			{
				break;
				//TODO check error WIN \ UNIX
			}
		} while (length > 0);
	}

	MessageType Protocol::RecieveAnswer(int socket)
	{
		char recvBuffer[RESPONSE_OK_LENGTH];

		int recieved = 0;
		MessageType response = eNotSet;
		do
		{
			recieved = recv(socket, recvBuffer, RESPONSE_OK_LENGTH, NULL);
			if (recieved == 0)
			{
				//TODO check error or close conn
			}
			else if (recieved == -1)
			{
				//TODO checkerror
			}
			else
			{
				return response = GetMessageType(recvBuffer);
			}
			//TODO maybe while responce != oK
		}
		//while (recieved < RESPONSE_OK_LENGTH);
		//TODO check 
		while (true);
	}

	void Protocol::SendResponse(MessageType messageType, int socket, std::string* pstrMessage)
	{
		Message message(messageType, pstrMessage);
		SendMessagee(message, socket);
	}

	bool Protocol::IsLegalPackage(char* buff)
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

	MessageType Protocol::GetMessageType(char* buff)
	{
		if (*(int*)buff == HEADER_START)
		{
			//MessageType messageType = buff[MESSAGE_TYPE_INDEX];
			
			switch (buff[MESSAGE_TYPE_INDEX])
			{
				case eNameRequest:
				case eMessageRequest:
				case eResponseOk:
				case eResponceError:
					return (MessageType)buff[MESSAGE_TYPE_INDEX];
				default:
					throw new std::exception(" Not filled package type or package is trash \n");
			//case eRequest:

			//	if (buff[CONTENT_TYPE_INDEX] == eSendName)
			//	{
			//		return eNameRequest;
			//	}
			//	else if (buff[CONTENT_TYPE_INDEX] == eSendMessage)
			//	{
			//		return eMessageRequest;
			//	}
			//	break;
			//case eResponce:
			//	if (buff[CONTENT_TYPE_INDEX] == eResponseOk)
			//	{
			//		return eResponseOk;
			//	}
			//	else if (buff[CONTENT_TYPE_INDEX] == eResponceError)
			//	{
			//		return eResponceError;
			//	}
			//	break;

			}
		}
		throw new std::exception(" It isn`t message \n");
		//MesageType Protocol::CheckPackageType()
	}

	int IncomingMessageNum(std::vector<int> &socketVec)
	{
		fd_set rfds;
		FD_ZERO(&rfds);

		std::vector<int>::iterator it;

		int maxNumFD = 0;
		for (it = socketVec.begin(); it != socketVec.end(); it++)
		{
			FD_SET(*it, &rfds);
			maxNumFD = max(maxNumFD, *it);
		}
		maxNumFD++;

		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 500;

		Message message;

		int result = select(maxNumFD, &rfds, NULL, NULL, &timeout);
		return result;
	}
	//int IncomingMessageNum(int &sockfd)
	//{
	//	fd_set rfds;
	//	FD_ZERO(&rfds);
	//	FD_SET(sockfd, &rfds);
	//	int maxNumFD = sockfd + 1;

	//	timeval timeout;
	//	timeout.tv_sec = 0;
	//	timeout.tv_usec = 500;

	//	Message message;

	//	int result = select(maxNumFD, &rfds, NULL, NULL, &timeout);
	//	return result;
	//}

}

#include "stdafx.h"
#include "Protocol.h"
#include <iostream>

namespace ChatLib
{
	Response Protocol::TrySendMessage(Message message, int socket)
	{
		//TODO need to know sent message or not
		SendMessagee(message, socket);

		Response response = RecieveAnswer(socket);

		return response;
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
				Message message = RecieveMessageFromServer(socket);

				SendResponse(eOk, socket);

				return message;
			}
		}
	}

	Message Protocol::RecieveMessageFromServer(int socket)
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
			std::string str;

			//TODO refactor It must be inside message
			//TODO maybe lost data after recieve

			MessageType type = GetMessageType(buff);

			switch (type)
			{
			case eNameRequest:
				str.append(buff, HEADER_SIZE + buff[MESSAGE_LENGTH_INDEX]);
				break;
			case eMessageRequest:
				str.append(buff, HEADER_SIZE + buff[MESSAGE_LENGTH_INDEX]);
				break;
			case eResponseOk:
				str.append(buff, HEADER_SIZE);
				break;
			case eResponceError:
				str.append(buff, HEADER_SIZE);
				break;
			}
			//TODO THIS______________________________________________________________________
			Message message(str);
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

	Response Protocol::RecieveAnswer(int socket)
	{
		char recvBuffer[RESPONSE_OK_LENGTH];

		int recieved = 0;
		Response response = eNotSet;
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
				return response = CheckResponseStatus(recvBuffer);
			}
			//TODO maybe while responce != oK
		}
		//while (recieved < RESPONSE_OK_LENGTH);
		//TODO check 
		while (true);
	}

	void Protocol::SendResponse(Response responceVal, int socket, std::string* pstrMessage)
	{
		Message message(responceVal, pstrMessage);
		SendMessagee(message, socket);
	}

	bool Protocol::IsLegalPackage(char* buff)
	{
		return false;
	}

	Response Protocol::CheckResponseStatus(char* buff)
	{
		if (*(int *)buff == HEADER_START)
		{
			//TODO check
			if ((int)buff[4] == eResponce)
			{
				if ((int)buff[5] == eOk)
					return eOk;
				else if ((int)buff[5] == eError)
					return eError;
				else
				{
					//TODO
					//throw new std::exception(" Wrong Response \n");
					//std::cout << " Wrong Response \n";
				}
			}
		}
		else
		{
			//TODO exception
		}
	}

	MessageType Protocol::GetMessageType(char* buff)
	{
		if (*(int*)buff == HEADER_START)
		{
			switch (buff[MESSAGE_TYPE_INDEX])
			{
			case eRequest:

				if (buff[CONTENT_TYPE_INDEX] == eSendName)
				{
					return eNameRequest;
				}
				else if (buff[CONTENT_TYPE_INDEX] == eSendMessage)
				{
					return eMessageRequest;
				}
				break;
			case eResponce:
				if (buff[CONTENT_TYPE_INDEX] == eResponseOk)
				{
					return eResponseOk;
				}
				else if (buff[CONTENT_TYPE_INDEX] == eResponceError)
				{
					return eResponceError;
				}
				break;
			default:
				throw new std::exception(" Not filled package type or package is trash \n");
			}
		}
		//MesageType Protocol::CheckPackageType()
	}
}

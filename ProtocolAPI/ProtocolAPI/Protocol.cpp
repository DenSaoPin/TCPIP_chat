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
				Message message = RecieveMessage(socket);

				SendResponse(eOk, socket);

				return message;
			}
		}
	}

	Message Protocol::RecieveMessage(int socket)
	{
		//TODO check revieved num
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
			if (*(int*)buff == HEADER_START)
			{
				switch (buff[4])
				{
				case eRequest:
					str.append(buff, HEADER_SIZE + buff[6]);
					break;
				case eResponce:
					str.append(buff, HEADER_SIZE);
					break;
				default:
					throw new std::exception(" Not filled package type or package is trash \n");
				}

				switch (buff[5])
				{
				case eSendName:
					break;
				case eSendMessage:
					break;
				default:
					throw new std::exception(" Not filled sixth header element or package is trash \n");
				}
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
		}
		while (length > 0);
	}

	Response Protocol::RecieveAnswer(int socket)
	{
		char recvBuffer[RESPONSE_OK_LENGTH];

		int recieved = 0;
		Response response;
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
				if (*(int*)recvBuffer == HEADER_START)
				{
					//TODO check
					if ((int)recvBuffer[4] == eResponce)
					{
						if ((int)recvBuffer[5] == eOk)
							return eOk;
						else if ((int)recvBuffer[5] == eError)
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
			//TODO maybe while responce != oK
		}
		//while (recieved < RESPONSE_OK_LENGTH);
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
}

// TCPIP_Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma comment (lib, "Ws2_32.lib")

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <string>
#include "ProtocolAPI.h"
#include <random>

//#include <sys/types.h>
//#include <sys/socket.h>

#define MAX_NAME_LENGTH 255
#define MAX_PORT_DIGIT 5
#define MAX_PACKAGE_LENGTH 255

#define HEADER_START 0xFFFFFFFF
#define MESSAGE_START_INDEX 7
#define RESPONSE_OK_LENGTH 6

class Message//:std::string
{
public:
	std::string Package;
	std::string MessageText;
	//std::string MessageID;

	Message(Response responseVal, std::string *pstrMessage = nullptr)
	{
		CreateHeader(eResponce, responseVal);
		if (pstrMessage != nullptr)
		{
			MessageText = *pstrMessage;
			Package.push_back(MessageText.length());
			Package.append(MessageText);
		}
	}

	Message(Request requestVal, std::string message)
	{
		CreateHeader(eResponce, requestVal);

		MessageText = message;
		Package.push_back(message.length());
		Package.append(message);
	}
	Message(std::string str)
	{
		//TODO check copy or ref
		Package = str;
	}
	Message()
	{
		CreateHeader();
	}


private:
	char _mStartHeaderPoint[4] = { (char)0xFF, (char)0xFF, (char)0xFF, (char)0xFF };
	PackageType _mPackageType;
	Response _mResponse;
	Request _mRequest;

	void CreateHeader(PackageType type, Request requestVal)
	{
		Package.append(_mStartHeaderPoint, 4);
		Package.push_back((char)eResponce);
		Package.push_back((char)requestVal);
	}
	void CreateHeader(PackageType type, Response responseVal)
	{
		Package.append(_mStartHeaderPoint, 4);
		Package.push_back((char)eResponce);
		Package.push_back((char)responseVal);
	}
	void CreateHeader()
	{
		Package.append(_mStartHeaderPoint, 4);
	}
};

class UIInterface
{
public:
	std::string GetName()
	{
		std::string ret;
		std::cout << "Enter your name: \n";
		std::cin >> ret;
		return  ret;
	}
	std::string GetIP()
	{
		std::cout << "Enter Server IP or press Enter for use localhost \n" << std::endl;

		//TODO Do we have best clean \n method?
		std::cin.ignore();

		//TODO check 
		std::string ret;
		std::getline(std::cin, ret);

		//TODO check
		if (ret.length() == 0)
		{
			ret = "127.0.0.1";
		}
		else if (ret.length() > INET_ADDRSTRLEN)
		{
			throw std::exception("Invalid IPv4 address");
		}

		return  ret;
	}
	std::string GetPort()
	{
		std::string ret;
		std::cout << "Enter Server Port or press Enter for use default 7700 \n" << std::endl;
		std::getline(std::cin, ret);

		//TODO check
		if (ret.length() == 0)
		{
			ret = "7700";
		}
		else if (ret.length() > MAX_PORT_DIGIT)
		{
			throw std::exception("Invalid IPv4 port");
		}
		return  ret;
	}
	void PrintMessage(std::string str)
	{
		std::cout << str;
	}
	void PrintMessage(Message msg)
	{
		std::cout << msg.MessageText;
	}
};

	class TCPIP_Client
	{
	public:

		std::string Name;
		std::string ServerIP;
		std::string ServerPort;
		int Socket = NULL;
		//Response TryRecieveMessage(char*);

		//void CallInitializeDialog(char * nameBuff, char * serverIPbuff, char * serverPortBuff);

		//int InitializeSocketRoutine();

		//TODO why const and & 
		//TODO don`t copy?
		//Response TrySendMessage(const std::string& cs);
		
		TCPIP_Client(std::string name, std::string serverIP, std::string serverPort)
		{
			Name = name;
			ServerIP = serverIP;
			ServerPort = serverPort;
		}

		Response IntroduceToServer()
		{
			if (Name.length() == 0)
				throw new std::exception("You must fill client.Name before call IntroduceToServer() \n");

			Message message(eSendName, Name);

			Response response;
			do
			{
				response = TrySendMessage(message);
			} while (response != eOk);
			return response;
		}

		Response TrySendMessage(Message message)
		{
			SendMessagee(message);

			Response response = RecieveAnswer();

			return response;
		}

		Message TryRecieveMessage()
		{
			fd_set rfds;
			FD_ZERO(&rfds);
			FD_SET(Socket, &rfds);
			int maxNumFD = Socket;

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
				if (FD_ISSET(Socket, &rfds))
				{
					Message message = RecieveMessage();

					SendResponse(eOk);

					return message;
				}
			}
		}

		Message RecieveMessage()
		{
			//TODO check revieved num
			char buff[MAX_PACKAGE_LENGTH];
			int recived = recv(Socket, buff, sizeof buff, NULL);
			if(recived == 0)
			{
				//TODO error check Win and Timeout
			}else if( recived == -1)
			{
				//TODO check Unix error
			}else
			{

				//TODO refactor It must be inside message
				if (IsLegalPackage(buff))
				{
					Message message(buff);
					return message;
				}
				else
				{
					throw new std::exception("It isn`t message");
				}
			}

		}

		void SendMessagee(Message message)
		{
			int sended = 0;
			int length = message.Package.length();
			do
			{
				//TODO check with length = 1
				sended = send(Socket, &(message.Package.c_str()[message.Package.length() - length]), length, NULL);
				length -= sended;
				if (sended == -1)
				{
					break;
					//TODO check error WIN \ UNIX
				}
			} while (length > 0);
		}

		Response RecieveAnswer()
		{
			char recvBuffer[RESPONSE_OK_LENGTH];

			int recieved = 0;
			Response response;
			do
			{
				recieved = recv(Socket, recvBuffer, RESPONSE_OK_LENGTH, NULL);
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
					if (*recvBuffer == HEADER_START)
					{
						//TODO check
						if((int)recvBuffer[4] == eResponce)
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
					}else
					{
						//TODO exception
					}
				}
			} while (recieved < RESPONSE_OK_LENGTH);
		}

		void SendResponse(Response responceVal, std::string *pstrMessage = nullptr)
		{
			Message message(responceVal, pstrMessage);
			SendMessagee(message);
		}

		bool IsLegalPackage(char *buff)
		{
			if ((int)buff == HEADER_START)
			{
				switch (buff[4])
				{
				case eRequest:
					break;
				case eResponce:
					break;
				default:
					throw new std::exception(" Not filled package type or package is trash \n");
					return false;
				}

				switch (buff[5])
				{
				case eSendName:
					break;
				case eSendMessage:
					break;
				default:
					throw new std::exception(" Not filled sixth header element or package is trash \n");
					return false;
				}
				return true;
			}
		}


		void InitializeSocketRoutine()
		{
			WSADATA wsaData;

			if (WSAStartup(MAKEWORD(2, 2), &wsaData))
			{
				std::cout << "Could not initialize winsock \n" << std::endl;
			}

			struct addrinfo hints;
			struct addrinfo *servinfo;

			memset(&hints, 0, sizeof hints);

			hints.ai_family = PF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			//hints.ai_flags = AI_PASSIVE;


			//TODO check errors
			if (int status = getaddrinfo(ServerIP.c_str(), ServerPort.c_str(), &hints, &servinfo) != 0)
			{
				std::cout << "getaddrinfo error: \n" << gai_strerror(status) << std::endl;
			}


			Socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

			//TODO maybe need bind

			//connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
			if (int status = connect(Socket, servinfo->ai_addr, servinfo->ai_addrlen) != 0)
			{
				std::cout << "connect error: " << gai_strerror(status) << std::endl;
			}

		}

		int GetSocket()
		{
			return Socket;
		}
	};
//TODO check
	int main()
	{
		setlocale(LC_ALL, "Russian");

		UIInterface ui;
		std::string confClientName = ui.GetName();
		std::string confServerIP = ui.GetIP();
		std::string confServerPort = ui.GetPort();

		TCPIP_Client client(confClientName, confServerIP, confServerPort);
		

		client.InitializeSocketRoutine();
		int sockfd = client.GetSocket();

		client.IntroduceToServer();

		char buff[1024];

		while (true)
		{
			//TODO how to delete right
			Message message = client.TryRecieveMessage();

			ui.PrintMessage(message);

			Message sendMessage;
			sendMessage.Package[4] = eRequest;
			sendMessage.Package[5] = eSendMessage;
			sendMessage.MessageText = " Test message \n";

			client.TrySendMessage(sendMessage);
		}

		closesocket(sockfd);
		std::cout << "Hello World!\n";

		return 0;
	}

	// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
	// Debug program: F5 or Debug > Start Debugging menu

	// Tips for Getting Started: 
	//   1. Use the Solution Explorer window to add/manage files
	//   2. Use the Team Explorer window to connect to source control
	//   3. Use the Output window to see build output and other messages
	//   4. Use the Error List window to view errors
	//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
	//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file




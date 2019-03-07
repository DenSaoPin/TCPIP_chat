#include "ServerClient.h"
#include "Server.h"
#include <iostream>

ServerClient::ServerClient(Server* pServer, SOCKET socket)
{
	m_pServer = pServer;
	Socket = socket;
}

bool ServerClient::ProcessSocket()
{
	fd_set rfds;

	FD_ZERO(&rfds);

	FD_SET(Socket, &rfds);

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1;

	int result = select(Socket + 1, &rfds, NULL, NULL, &tv);

	if (FD_ISSET(Socket, &rfds))
	{
		ChatLib::Message message(ChatLib::eInvalid);
		try
		{
			message = ChatLib::Protocol::RecieveMessage(Socket);
		}
		catch (ChatLib::Protocol::ConnectionClosedException& ex)
		{
			//TODO need implement print exception text 
			std::cout << "Exception catched" << std::endl;
			IInvalid = true;
			closesocket(Socket);
		}
		catch (ChatLib::Protocol::ConnectionLostException)
		{
		}
		switch (message.GetType())
		{
		case ChatLib::eNameRequest:
			{
				if (message.GetText() != "")
				{
					std::string strNameClient = message.GetText();
					if (m_pServer->Assign(strNameClient, this))
					{
					}


					if (Name != "")
					{
						ChatLib::Message msg(ChatLib::eMessageRequest,
						                     "This socket already have oldName - " + Name + " I can`t set newName" +
						                     strNameClient + "\n");

						ChatLib::Protocol::TrySendMessage(msg, Socket);
					}
					else
					{
						Name = strNameClient;

						printf("Name Responce \n");
						ChatLib::Protocol::SendResponse(ChatLib::eResponseOk, Socket);
					}
				}
				break;
			}
		case ChatLib::eMessageRequest:
			{
				if (message.GetText() != "")
				{
					if (Name != "")
					{
						std::cout << Name + ": " + message.GetText() << std::endl;
						//TODO send responce;

						printf("Message Responce \n");
						ChatLib::Protocol::SendResponse(ChatLib::eResponseOk, Socket);
					}
					else
					{
						std::cout << "Error socket don`t have Name" << std::endl;
					}
				}
				break;
			}
		case ChatLib::eResponseOk:
		case ChatLib::eResponceError:
			std::cout << "Responce in Client" << std::endl;
			break;
		case ChatLib::eInvalid:
			std::cout << "Invalid in Client" << std::endl;
			break;
		}
		std::cout << "Byte recieved: " << message.GetTextLength() << std::endl;
		//m_log->info("Received %d bytes", message.GetTextLength());
	}
	//std::chrono time;
	return true;
}

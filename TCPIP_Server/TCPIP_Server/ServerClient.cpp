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
	fd_set rfds, wfds;

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);

	FD_SET(Socket, &rfds);

	if(!ForSend.empty())
	{
		FD_SET(Socket, &wfds);
	}

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1;

	int result = select(Socket + 1, &rfds, &wfds, NULL, &tv);

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
			//TODO not implemented
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
						printf("Name Responce \n");
						ChatLib::Protocol::SendResponse(ChatLib::eResponseOk, Socket);
					}
					else
					{
						ChatLib::Message msg(ChatLib::eMessageRequest,
							"This name already in use - " + Name + "\n");
						ChatLib::Protocol::TrySendMessage(msg, Socket);
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
						std::cout << message.GetText() << std::endl;
						//TODO send responce;

						m_pServer->SetToSendForAllClients(this, message);

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

	if(FD_ISSET(Socket, &wfds))
	{
		ChatLib::MessageType type = ChatLib::Protocol::TrySendMessage(ForSend.front(), Socket);
		if(type == ChatLib::eResponseOk)
		{
			ForSend.pop();
		}
		//TODO refactored MEssage Type to status and type
	}
	return true;
}

#include "ServerClient.h"
#include <iostream>
#include <string>
#include "../../ProtocolAPI/ProtocolAPI/NameRequestMessage.h"
#include "loggerAPI/LoggerManager.h"
#include "TCPServer.h"
#include "Exceptions.h"

ServerClient::ServerClient(TCPServer* pServer, SOCKET socket)
{
	m_log = LoggerManager::GetLogger("ServerClient");
	m_log->error("Test");
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
		ChatLib::RawBytes rawMessage;
		try
		{
			rawMessage = m_pServer->RecieveMessage(Socket);
		}
		catch (Exceptions::ConnectionClosedException& ex)
		{
			//TODO need implement print exception text 
			std::cout << "Exception ConnectionClosedException catched" << std::endl;
			IInvalid = true;
			closesocket(Socket);
			return false;
		}
		catch (Exceptions::ConnectionLostException)
		{
			std::cout << "Exception ConnectionLostException catched" << std::endl;
			IInvalid = true;
			closesocket(Socket);
			return false;
		}

		ChatLib::byte *p = &rawMessage[0];

		switch (ChatLib::BaseMessage::GetType(rawMessage))
		{
			case ChatLib::eNameRequest:
		{
			const ChatLib::NameRequestMessage NameMessage(p);
			ChatLib::ResponseStatus status = ChatLib::eOk;
			m_log->info("Received message eNameRequest for %s", NameMessage.Text.c_str());

			if (!m_pServer->Assign(NameMessage.Text, this))
			{
				status = ChatLib::eNameConflict;
				IInvalid = true;
			}

			m_pServer->SendResponse(status, Socket);
			break;
		}
		case ChatLib::eBroadcastMessage:
		{
			//ChatLib::BroadcastMessage BroadMessage(p);

			ChatLib::BroadcastMessagePtr broadMessagePtr(new ChatLib::BroadcastMessage(p));

			if (!broadMessagePtr->Text.empty())
			{
				if (!Name.empty())
				{
					std::cout << broadMessagePtr->Text << std::endl;
					//TODO send responce;
					m_pServer->SetToSendForAllClients(this, broadMessagePtr);

					printf("Message Responce \n");
					m_pServer->SendResponse(ChatLib::ResponseStatus::eOk, Socket);
				}
				else
				{
					std::cout << "Error socket don`t have Name" << std::endl;
				}
			}
			break;
		}
		case ChatLib::eDirectMessage:
		{
			//ChatLib::DirectMessage DirMessage(p);
			ChatLib::DirectMessagePtr directMessagePtr(new ChatLib::DirectMessage(p));

			if (!directMessagePtr->Text.empty())
			{
				if (!Name.empty())
				{
					//std::cout << directMessagePtr->Text << std::endl;
					//TODO send responce;

					m_pServer->SetToSendFor(m_pServer->Clients.Find(directMessagePtr->TargetName), directMessagePtr);

					printf("Message Responce \n");
					m_pServer->SendResponse(ChatLib::ResponseStatus::eOk, Socket);
				}
				else
				{
					std::cout << "Error socket don`t have Name" << std::endl;
				}
			}
			break;
		}
		//case ChatLib::eResponseOk:
		//case ChatLib::eResponceError:
		//	std::cout << "Responce in Client" << std::endl;
		//	break;
		//case ChatLib::MessageType::e
		//	std::cout << "Invalid in Client" << std::endl;
		//	break;
		case ChatLib::eInvalid:
			std::cout << "Invalid in Client" << std::endl;
			break;
		}
		//std::cout << "Byte recieved: " << message.GetTextLength() << std::endl;
		//m_log->info("Received %d bytes", message.GetTextLength());
	}

	if(FD_ISSET(Socket, &wfds))
	{
		ChatLib::Response response = m_pServer->TrySendMessage(ForSend.front().get(), Socket);

		if(response.GetStatus() == ChatLib::eOk)
		{
			ForSend.pop();
		}
		//TODO refactored MEssage Type to status and type
	}
	return true;
}

#include "ServerClient.h"
#include "Server.h"
#include <iostream>
#include "../../ProtocolAPI/ProtocolAPI/NameRequestMessage.h"

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
		ChatLib::RawBytes rawMessage;
		try
		{
			rawMessage = ChatLib::Protocol::RecieveMessage(Socket);
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

		ChatLib::byte *p = &rawMessage[0];

		switch (ChatLib::BaseMessage::GetType(rawMessage))
		{
			case ChatLib::eNameRequest:
		{
			const ChatLib::NameRequestMessage NameMessage(p);
			ChatLib::ResponseStatus status = ChatLib::eOk;

			if (!m_pServer->Assign(NameMessage.Text, this))
				status = ChatLib::eNameConflict;

			ChatLib::Protocol::SendResponse(ChatLib::ResponseStatus::eOk, Socket);
			break;
		}
		case ChatLib::eBroadcastMessage:
		{
			//ChatLib::BroadcastMessage BroadMessage(p);

			ChatLib::BroadcastMessagePtr broadMessagePtr(new ChatLib::BroadcastMessage(p));

			if (!broadMessagePtr->Text.empty())
			{
				if (Name != "")
				{
					std::cout << broadMessagePtr->Text << std::endl;
					//TODO send responce;
					m_pServer->SetToSendForAllClients(this, broadMessagePtr);

					printf("Message Responce \n");
					ChatLib::Protocol::SendResponse(ChatLib::ResponseStatus::eOk, Socket);
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
				if (Name != "")
				{
					std::cout << directMessagePtr->Text << std::endl;
					//TODO send responce;


					m_pServer->SetToSendFor(this, directMessagePtr);

					printf("Message Responce \n");
					ChatLib::Protocol::SendResponse(ChatLib::ResponseStatus::eOk, Socket);
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
		ChatLib::Response response = ChatLib::Protocol::TrySendMessage(ForSend.front().get(), Socket);

		if(response.GetStatus() == ChatLib::eOk)
		{
			ForSend.pop();
		}
		//TODO refactored MEssage Type to status and type
	}
	return true;
}

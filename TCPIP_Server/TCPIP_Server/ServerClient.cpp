#include "ServerClient.h"
#include <iostream>
#include <string>
#include "../../ProtocolAPI/ProtocolAPI/NameRequestMessage.h"
#include "loggerAPI/LoggerManager.h"
#include "TCPServer.h"
#include "Exceptions.h"
#include <chrono>

ServerClient::ServerClient(TCPServer* pServer, SOCKET socket)
{
	m_log = LoggerManager::GetLogger("ServerClient");
	m_log->error("Test");
	m_pServer = pServer;
	Socket = socket;
}

//TODO move this
using Time = std::chrono::steady_clock;
Time::time_point loopStartTime;
using int_sec = std::chrono::duration<int>;
const int_sec timeGap{ 76 };

bool ServerClient::ProcessSocket()
{
	fd_set rfds, wfds;

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);

	FD_SET(Socket, &rfds);
	FD_SET(Socket, &wfds);

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
		case ChatLib::eInvalid:
		{
			std::cout << "Invalid in ServerClient main switch" << std::endl;
			break;
		}
		case ChatLib::eNameRequest:
		{
			const ChatLib::NameRequestMessagePtr NameMessagePtr(new ChatLib::NameRequestMessage(p));
			ChatLib::ResponseStatus status = ChatLib::eOk;
			m_log->info("Received message eNameRequest for %s", NameMessagePtr->Text.c_str());

			if (!m_pServer->Assign(NameMessagePtr->Text, this))
			{
				status = ChatLib::eNameConflict;
				IInvalid = true;
			}

			m_pServer->SetResponse(status, *this, NameMessagePtr->GetMyID());
			break;
		}
		case ChatLib::eDirectMessage:
		{
			ChatLib::DirectMessagePtr directMessagePtr(new ChatLib::DirectMessage(p));

			if (!directMessagePtr->Text.empty())
			{
				if (!Name.empty())
				{
					m_pServer->SetToSendFor(m_pServer->Clients.Find(directMessagePtr->TargetName), directMessagePtr);

					printf("Message Responce \n");
					m_pServer->SetResponse(ChatLib::ResponseStatus::eOk, *this, directMessagePtr->GetMyID());
				}
				else
				{
					std::cout << "Error socket don`t have Name" << std::endl;
				}
			}
			break;
		}
		case ChatLib::eBroadcastMessage:
		{
			ChatLib::BroadcastMessagePtr broadMessagePtr(new ChatLib::BroadcastMessage(p));

			if (!broadMessagePtr->Text.empty())
			{
				if (!Name.empty())
				{
					std::cout << broadMessagePtr->Text << std::endl;
					m_pServer->SetToSendForAllClients(this, broadMessagePtr);

					printf("Message Responce \n");

					m_pServer->SetResponse(ChatLib::ResponseStatus::eOk, *this, broadMessagePtr->GetMyID());
				}
				else
				{
					std::cout << "Error socket don`t have Name" << std::endl;
				}
			}
			break;
		}
		case ChatLib::eResponse:
		{
			ChatLib::ResponsePtr responsePtr(new ChatLib::Response(p));
			if (this->ForSend.front()->GetMyID() == responsePtr->GetMyID())
			{
				this->ForSend.pop();
			}
			else
			{
				std::cout << "Warning: Unhandled response in serverClient processing \n";
			}
			break;
		}
		default:
			std::cout << "Default in ServerClient main switch" << std::endl;
			break;
		}
	}
	   
	if( (!this->ForSend.empty()) &&
		FD_ISSET(Socket, &wfds)
		)
	{
		ChatLib::BaseMessagePtr currentMessage = this->ForSend.front();

		if (currentMessage == awaitResponse)
		{
			if (Time::now() > loopStartTime + timeGap)
			{
				m_pServer->SendMessagee(currentMessage.get(), Socket);
				awaitResponse = currentMessage;
				loopStartTime = Time::now();
			}
		}
		else
		{
			m_pServer->SendMessagee(currentMessage.get(), Socket);
			awaitResponse = currentMessage;
		}

		if (ForSend.front()->GetMyType() == ChatLib::eResponse)
			ForSend.pop();
	}
	return true;
}

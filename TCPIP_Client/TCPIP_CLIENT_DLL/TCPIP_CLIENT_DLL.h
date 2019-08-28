#pragma once

#include <string>
#include <queue>
#include <mutex>

#include "ProtocolAPI/BaseMessage.h"
#include "ProtocolAPI/Response.h"
#include "Defines.h"
#include "public/ChatClientAPI.h"

#define MAX_PORT_DIGIT 5



	class TCPIP_Client
	{
	private:
		TCPIP_Client() {};
		~TCPIP_Client() {};
		TCPIP_Client(const TCPIP_Client&);                 // Prevent copy-construction
		TCPIP_Client& operator=(const TCPIP_Client&);

		static TCPIP_Client* _instance;

		std::queue<ChatLib::BaseMessagePtr> m_outgoingMessages;
		ChatLib::BaseMessagePtr awaitResponse;

		unsigned short m_currentMessageId = 0;

		ThreadSafe <EClientStatus> m_ClientStatus = eStartWSA;
	public:

		std::string Name;
		std::string ServerIP;
		std::string ServerPort;

		int Socket = NULL;

		static TCPIP_Client* Instance();

		void Initialize(const std::string& name, const std::string& serverIP, const std::string& serverPort);

		bool InitializeSocketRoutine();
		bool InitWinSockDll();

		void ShowError(const std::string &);

		int GetSocket();

		EClientStatus GetStatus();
		void ClientMainLoop();
		void Shutdown();

		ChatLib::RawBytes TCPIP_Client::RecieveMessage(const CROSS_SOCKET& socket);

		bool TCPIP_Client::SendMessagee(ChatLib::BaseMessagePtr message, const CROSS_SOCKET& socket);

		void TCPIP_Client::AddForSend(std::string& sz_target_name, const int status, const void* data, const int data_len);

		void TCPIP_Client::SetResponse(ChatLib::ResponseStatus status, const unsigned short &id);

		unsigned short GenerateId();
	};

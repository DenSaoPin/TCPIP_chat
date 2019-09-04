#pragma once

#include <string>
#include <queue>
#include <mutex>

#include "ProtocolAPI/BaseMessage.h"
#include "ProtocolAPI/Response.h"
#include "ProtocolAPI/NameRequestMessage.h"
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
        ChatLib::NameRequestMessagePtr m_NameRequest = nullptr;

		unsigned short m_currentMessageId = 0;
        ThreadSafe <EClientStatus> m_ClientStatus{eStartWSA};
	public:

		std::string Name;
		std::string ServerIP;
		std::string ServerPort;

        CROSS_SOCKET Socket = 0;

		static TCPIP_Client* Instance();

		void Initialize(const std::string& name, const std::string& serverIP, const std::string& serverPort);

		bool InitializeSocketRoutine();

		bool InitWinSockDll();

        void SocketCleanup();

		void ShowError(const std::string &);

		int GetSocket();

		EClientStatus GetStatus();
	    //bool IntroduceToServer();
		void ClientMainLoop();
		void Shutdown();

        ChatLib::RawBytes RecieveMessage(const CROSS_SOCKET& socket);

        bool SendMessagee(ChatLib::BaseMessagePtr message, const CROSS_SOCKET& socket);

        void AddForSend(std::string& sz_target_name, const int status, const void* data, const int data_len);

        void SetResponse(ChatLib::ResponseStatus status, const unsigned short &id);

		unsigned short GenerateId();
	};

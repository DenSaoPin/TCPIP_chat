#pragma once

#include <ProtocolAPI/Protocol.h>
#include <string>
#include <queue>

#define MAX_PORT_DIGIT 5

	class TCPIP_Client
	{
	private:
		TCPIP_Client() {};
		~TCPIP_Client() {};
		TCPIP_Client(const TCPIP_Client&);                 // Prevent copy-construction
		TCPIP_Client& operator=(const TCPIP_Client&);

		static TCPIP_Client* _instance;
		std::queue<std::string> m_outgoingMessages;
		bool m_NeedTerminate = false;
		bool m_IsTerminate = false;

	public:
		std::string Name;
		std::string ServerIP;
		std::string ServerPort;

		int Socket = NULL;
		bool IsStarted = false;

		static TCPIP_Client* Instance();

		void Initialize(const std::string& name, const std::string& serverIP, const std::string& serverPort);

		ChatLib::Response IntroduceToServer();

		void InitializeSocketRoutine();

		int GetSocket();

		void ClientMainLoop();
		void SendTextMessage(const char* sz_str);
		void Shutdown();

		//const char* ClientSendMessage(const char *);
};

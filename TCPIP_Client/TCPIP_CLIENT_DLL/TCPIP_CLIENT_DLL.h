#pragma once

#include "stdafx.h"

#pragma comment (lib, "Ws2_32.lib")

#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <cstddef>
#include "../../ProtocolAPI/ProtocolAPI/Protocol.h"

#define MAX_PORT_DIGIT 5

	class TCPIP_Client
	{
	private:
		TCPIP_Client() {};
		~TCPIP_Client() {};
		TCPIP_Client(const TCPIP_Client&);                 // Prevent copy-construction
		TCPIP_Client& operator=(const TCPIP_Client&);

		static TCPIP_Client* _instance;

	public:
		std::string Name;
		std::string ServerIP;
		std::string ServerPort;
		int Socket = NULL;

		char* szHasIncomingMessage = nullptr;
		bool NeedTerminate = false;

		static TCPIP_Client* Instance();

		void Initialize(const std::string& name, const std::string& serverIP, const std::string& serverPort);

		ChatLib::Response IntroduceToServer();

		void InitializeSocketRoutine();

		int GetSocket();

		void ClientMainLoop();

		//const char* ClientSendMessage(const char *);
};

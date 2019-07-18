#pragma once

#include "stdafx.h"

#pragma comment (lib, "Ws2_32.lib")

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include <future>
#include <cstddef>
#include "../../ProtocolAPI/ProtocolAPI/Protocol.h"
#include "../../ProtocolAPI/ProtocolAPI/BroadcastMessage.h"
#include "../../ProtocolAPI/ProtocolAPI/DirectMessage.h"
#include "../../ProtocolAPI/ProtocolAPI/NameRequestMessage.h"
#include "CallbacksHolder.h"

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

		static TCPIP_Client* Instance();

		void Initialize(const std::string& name, const std::string& serverIP, const std::string& serverPort);

		ChatLib::Response IntroduceToServer();

		void InitializeSocketRoutine();

		int GetSocket();

		void ClientMainLoop();
	};

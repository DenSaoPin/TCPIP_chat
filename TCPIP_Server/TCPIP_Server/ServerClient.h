#pragma once
#include "../../ProtocolAPI/ProtocolAPI/Protocol.h"
#include <queue>
#include "TCPServer.h"

class TCPServer;

class ServerClient
{
	ILogger *m_log;
	TCPServer *m_pServer;
public:
	ServerClient(TCPServer* pServer, CROSS_SOCKET socket);

	std::string Name = "";
	CROSS_SOCKET Socket = NULL;
	bool IInvalid = false;

	std::queue<ChatLib::BaseMessagePtr> ForSend;

	bool ProcessSocket();
};

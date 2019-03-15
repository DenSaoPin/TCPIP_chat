#pragma once
#include "../../ProtocolAPI/ProtocolAPI/Protocol.h"
#include <queue>

class Server;

class ServerClient
{
	Server *m_pServer;
public:
	ServerClient(Server* pServer, CROSS_SOCKET socket);

	std::string Name = "";
	CROSS_SOCKET Socket = NULL;
	bool IInvalid = false;

	std::queue<ChatLib::BaseMessage*> ForSend;

	bool ProcessSocket();
};

#pragma once
#include <xstring>
#include "ClientSet.h"
#include "../../ProtocolAPI/ProtocolAPI/Protocol.h"
#include <unordered_map>
#include <unordered_set>

class ServerClient;

class Server
{
public:
	static const char *DefaultAddress;
private:
	std::unordered_set<std::string> _iknowThisNames;
	int _connectSock;
	int _clientCount;
public:
	Server(std::string address, std::string port);
	
	bool Assign(const std::string& name, ServerClient* pClient);

	bool SetToSendForAllClients(ServerClient* server_client, ChatLib::Message& message);

	std::string ServerIP;
	std::string ServerPort;

	CROSS_SOCKET ListenSock = 0;

	ClientSet Clients;

	void InitialWSARoutine();

	void ListenSockInitialization(std::string& IPv4_Adress, std::string& port);

	void Accept();
};

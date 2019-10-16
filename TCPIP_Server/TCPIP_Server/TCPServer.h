#pragma once
#include <string>
#include "ClientSet.h"
#include <unordered_set>
#include "../../ProtocolAPI/ProtocolAPI/Protocol.h"
#include "../../ProtocolAPI/ProtocolAPI/DirectMessage.h"
#include "../../ProtocolAPI/ProtocolAPI/BroadcastMessage.h"
#include <loggerAPI/ILogger.h>
#include "ProtocolAPI/Response.h"
#include "Defines.h"

class ServerClient;
 
class TCPServer
{
public:
	static const char *DefaultAddress;
private:
	std::unordered_set<std::string> _iknowThisNames;
	ILogger* m_log;
	int _connectSock;
	int _clientCount;
public:
	TCPServer(std::string address, std::string port);

	bool Assign(const std::string& name, ServerClient* pClient);

	bool SetToSendForAllClients(ServerClient* server_client, ChatLib::BroadcastMessagePtr& broadcast_message);
	void SetToSendFor(ServerClient* server_client, ChatLib::DirectMessagePtr& direct_message);

	std::string ServerIP;
	std::string ServerPort;

	CROSS_SOCKET ListenSock = 0;

	ClientSet Clients;

	static void InitialWSARoutine();

	void ListenSockInitialization(std::string& IPv4_Adress, std::string& port);

	void Accept();

	//ChatLib::Response TrySendMessage(ChatLib::BaseMessage* message, const CROSS_SOCKET& socket);

	void SendMessagee(ChatLib::BaseMessage* message, const CROSS_SOCKET& socket);

	void SetResponse(ChatLib::ResponseStatus status, ServerClient& client, const unsigned short& id);

	ChatLib::RawBytes RecieveMessage(const CROSS_SOCKET& socket);

	void printWsaError();
    void printLinuxError();
};

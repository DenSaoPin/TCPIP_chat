#pragma once
#include <xstring>
#include "ClientSet.h"
#include "../../ProtocolAPI/ProtocolAPI/Protocol.h"

class ServerClient;

class Server
{
public:
	static const char *DefaultAddress;
private:
	int ConnectSock;
	//std::string strNameClient;
	int ClientCount;
public:

	Server(std::string address, std::string port);
	
	bool Assign(const std::string& name, ServerClient* pClient);

	std::string ServerIP;
	std::string ServerPort;

	//SocketsVector ConnectionSocketsVec;
	CROSS_SOCKET ListenSock = 0;

	//std::unordered_map<std::string, CROSS_SOCKET> mapClientNames;
	ClientSet Clients;

	void InitialWSARoutine();

	void ListenSockInitialization(std::string& IPv4_Adress, std::string& port);

	//int SetAllConections(fd_set* rfds, SocketsVector* connectionsVec = nullptr)
	//{
	//	FD_ZERO(rfds);
	//	//FD_SET(ListenSock, rfds);

	//	//int maxFD = (int)ListenSock + 1;
	//	int maxFD = 0;

	//	if (connectionsVec != nullptr)
	//	{
	//		for (SocketsVectorIterator it = connectionsVec->begin(); it != connectionsVec->end(); ++it)
	//		{
	//			//TODO check s is ref?
	//			//TODO maybe add in vector at this place
	//			CROSS_SOCKET s = *it;
	//			FD_SET(s, rfds);
	//			maxFD = maxFD > s ? maxFD : s + 1;
	//		}
	//	}
	//	else
	//	{
	//		for (auto itClients = Clients.Begin(); itClients != Clients.End(); ++itClients)
	//		{
	//			//TODO check s is ref?
	//			//TODO maybe add in vector at this place
	//			CROSS_SOCKET s = itClients->Socket;
	//			FD_SET(s, rfds);
	//			maxFD = maxFD > s ? maxFD : s + 1;
	//		}
	//	}
	//	return maxFD;
	//}

	void Accept();

};

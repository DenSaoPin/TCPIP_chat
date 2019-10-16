#include "ClientSet.h"
#include "ServerClient.h"
#include "exception"

ClientSet::ClientSet()
{
}


ClientSet::~ClientSet()
{
}

void ClientSet::Add(ServerClient* pClient)
{
	m_container.push_back(pClient);
}

void ClientSet::Delete(ServerClient* pClient)
{ 
	m_container.remove(pClient);
	delete pClient;
}

ClientSet::Iterator ClientSet::Begin()
{
	return m_container.begin();
}

ClientSet::Iterator ClientSet::End()
{
	return m_container.end();
}

bool ClientSet::Check(std::string inputName)
{
	int checkCounter = 0;
	for (auto itClient = Begin(); itClient != End(); itClient++)
	{
		if ((*itClient)->Name == inputName)
		{
			checkCounter++;
		}
	}
	if (checkCounter == 1)
	{
		return true;
	}
	else if(checkCounter == 0)
	{
		return false;
	}
	else
	{
        throw new std::runtime_error("Server finding clients error: More than one Client has the same Name descriptor");
	}
}

ServerClient* ClientSet::Find(const std::string &inputName)
{
	ServerClient* pOutputClient = nullptr;
	int checkCounter;
	for (auto itClients = Begin(); itClients != End(); ++itClients)
	{
		if ((*itClients)->Name == inputName)
		{
			return (*itClients);
		}
	}
	return nullptr;
}

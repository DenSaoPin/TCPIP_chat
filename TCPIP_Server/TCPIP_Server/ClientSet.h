#pragma once
#include <vector>
#include <list>


class ServerClient;

class ClientSet
{
	typedef std::list<ServerClient *>  Container;
	Container m_container;

public:
	ClientSet();
	~ClientSet();

	typedef Container::iterator  Iterator;

public:
	void		Add(ServerClient *pClient);
	void		Delete(ServerClient *pClient);
	Iterator	Begin();
	Iterator	End();
	bool		Check(std::string inputName);


	ServerClient* Find(const std::string &inputName);
};


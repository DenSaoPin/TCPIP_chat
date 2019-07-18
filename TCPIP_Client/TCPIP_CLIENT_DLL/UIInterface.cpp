#include "stdafx.h"
#include "UIInterface.h"

	std::string UIInterface::GetName()
	{
		std::string ret;
		std::cout << "Enter your name: \n";
		std::cin >> ret;
		return ret;
	}

	std::string UIInterface::GetIP()
	{
		std::cout << "Enter Server IP or press Enter for use localhost \n" << std::endl;

		//TODO Do we have best clean \n method?
		std::cin.ignore();

		//TODO check 
		std::string ret;
		std::getline(std::cin, ret);

		//TODO check
		if (ret.length() == 0)
		{
			ret = "127.0.0.1";
		}
		else if (ret.length() > INET_ADDRSTRLEN)
		{
			throw std::exception("Invalid IPv4 address");
		}
		return ret;
	}

	std::string UIInterface::GetPort()
	{
		std::string ret;
		std::cout << "Enter Server Port or press Enter for use default 7700 \n" << std::endl;
		std::getline(std::cin, ret);

		//TODO check
		if (ret.length() == 0)
		{
			ret = "7700";
		}
		else if (ret.length() > MAX_PORT_DIGIT)
		{
			throw std::exception("Invalid IPv4 port");
		}
		return  ret;
	}

	void UIInterface::PrintMessage(const std::string& str)
	{
		std::cout << str;
	}
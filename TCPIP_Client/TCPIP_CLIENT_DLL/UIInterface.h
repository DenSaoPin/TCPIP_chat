#pragma once

#include "TCPIP_CLIENT_DLL.h"
#include <string>


class UIInterface
{
public:
	std::string GetName();
	std::string GetIP();
	std::string GetPort();
	void PrintMessage(const std::string& str);
};

﻿#pragma once

#include <string>
#include <iostream>


class UIInterface
{
public:
	std::string GetName();
	std::string GetIP();
	std::string GetPort();
	void PrintMessage(const std::string& str);
};

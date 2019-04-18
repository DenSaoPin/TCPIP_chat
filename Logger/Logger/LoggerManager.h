#pragma once
#include "ILogger.h"

class DLLEXPORT LoggerManager
{
public:
	static ILogger *GetLogger(const char * name);
};

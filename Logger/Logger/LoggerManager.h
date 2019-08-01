#pragma once
#include "loggerAPI/ILogger.h"

class DLLEXPORT LoggerManager
{
public:
	static ILogger *GetLogger(const char * name);
};

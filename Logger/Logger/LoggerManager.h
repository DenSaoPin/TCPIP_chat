#pragma once
#include "loggerAPI/ILogger.h"

class Logger_PUBLIC LoggerManager
{
public:
	static ILogger *GetLogger(const char * name);
};

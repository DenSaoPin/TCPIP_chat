#pragma once
#include "LoggerManager.h"
#include "ILogger.h"

class LoggerManager
{
public:
	Logger_PUBLIC static ILogger *GetLogger(const char * name);
};

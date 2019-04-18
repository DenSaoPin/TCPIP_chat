#pragma once
#include "LoggerManager.h"
#include "ILogger.h"

class LoggerManager
{
public:
	DLLEXPORT static ILogger *GetLogger(const char * name);
};

#include "stdafx.h"

#include "LoggerManager.h"
#include "ConsoleLogger.h"
ILogger* LoggerManager::GetLogger(const char* name)
{
	return new ConsoleLogger(name);
}

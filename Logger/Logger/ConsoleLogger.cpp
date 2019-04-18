#include "stdafx.h"

#include "ConsoleLogger.h"
#include <cstdio>

ConsoleLogger::ConsoleLogger(const char* name): BaseLogger(name)
{
}

void ConsoleLogger::write(const char* pBuff)
{
	printf(pBuff);
}

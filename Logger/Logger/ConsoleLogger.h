#pragma once
#include "BaseLogger.h"

class ConsoleLogger : public BaseLogger
{

public:
	//int Print(ELogLevel debugLevel, const char *formatedText, ...);

public:
	explicit ConsoleLogger(const char* name);

protected:
	void write(const char*) override;
};

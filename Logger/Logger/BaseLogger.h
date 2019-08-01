#pragma once
#include "loggerAPI/ILogger.h"


class BaseLogger : public ILogger
{
	char message[1024];
	char *m_name;
public:
	BaseLogger(const char *name);
	virtual  ~BaseLogger();
public:
	void error(const char*, ...) override;
	void warn(const char*, ...) override;
	void info(const char*, ...) override;
protected:
	virtual void write(const char *) = 0;
private:
	void log(ELogLevel level, const char*);
};

#include "BaseLogger.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>

BaseLogger::BaseLogger(const char* name)
{
	m_name = nullptr;
	if (name != nullptr)
	{
		int len = strlen(name);
		m_name = new char[len + 1];
		memcpy(m_name, name, len);
		m_name[len] = 0x00;
	}
}

BaseLogger::~BaseLogger()
{
	if (m_name != nullptr)
		delete[] m_name;
}
#define STR_VALUE(arg) #arg
#define LOG_FUNC_IMPL(function, logtype) \
void BaseLogger::function(const char *format, ...) \
{ \
	va_list args;\
	va_start(args, format);\
	vsprintf(message, format, args);\
	va_end(args);\
	log(logtype, message);\
}

LOG_FUNC_IMPL(error, ELogLevel::eError);
LOG_FUNC_IMPL(warn, ELogLevel::eWarn);
LOG_FUNC_IMPL(info, ELogLevel::eInfo);


void BaseLogger::log(ELogLevel level, const char *message)
{
#define BUFF_SIZE 1024
	char buff[BUFF_SIZE];

	switch (level)
	{
	case ELogLevel::eError:
		sprintf(buff, "ERROR> ");
		break;
	case ELogLevel::eWarn:
		sprintf(buff, "WARNING> ");
		break;
	case ELogLevel::eInfo:
		sprintf(buff, "INFO> ");
		break;
	}
	if (m_name != nullptr)
	{
		sprintf(buff + strlen(buff), "%s: ", m_name);
	}
	sprintf(buff + strlen(buff), "%s\n", message);
	write(buff);
}

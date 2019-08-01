#pragma once

#if Logger_EXPORTS
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif

class DLLEXPORT ILogger
{
public:
	virtual ~ILogger() {};

	enum ELogLevel
	{
		eError,
		eWarn,
		eInfo,
	};
public:
	virtual void error(const char *, ...) = 0;
	virtual void warn(const char *, ...) = 0;
	virtual void info(const char *, ...) = 0;
};




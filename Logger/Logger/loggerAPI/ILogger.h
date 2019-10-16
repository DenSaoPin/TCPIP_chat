#pragma once

#if defined _WIN32 || defined __CYGWIN__
#ifdef Logger_EXPORTS
#ifdef __GNUC__
#define Logger_PUBLIC __attribute__ ((dllexport))
#else
#define Logger_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#endif
#else
#ifdef __GNUC__
#define Logger_PUBLIC __attribute__ ((dllimport))
#else
#define Logger_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#endif
#endif
#define DLL_LOCAL
#else
#if __GNUC__ >= 4
#define Logger_PUBLIC __attribute__ ((visibility ("default")))
#define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define Logger_PUBLIC
#define DLL_LOCAL
#endif
#endif

class Logger_PUBLIC ILogger
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




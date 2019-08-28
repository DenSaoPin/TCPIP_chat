#pragma once
#include <mutex>

#ifdef TCPIP_CLIENT_DLL_EXPORTS
#define TCPIPCLIENTDLL_EXPORT __declspec(dllexport)
#else
#define TCPIPCLIENTDLL_EXPORT __declspec(dllimport)
#endif

typedef void (*callbackMessageReceivedFunc) (const char *, const int*, const char*);

enum EClientStatus
{
	eInvalid            = 0,
	eStartWSA           = 1,
	eInitializeSocket   = 2,
	eIntroduce          = 3,
	eIdle               = 4,
	eTerminating        = 5,
	eShutDown           = 6,
};

extern "C"
{
	TCPIPCLIENTDLL_EXPORT void setCallbackMessageReceived(callbackMessageReceivedFunc);
	TCPIPCLIENTDLL_EXPORT void ClientMainLoop();
	TCPIPCLIENTDLL_EXPORT void ClientSendMessage(const char* szTargetName, const int status, const void * data, const int dataLen);
	TCPIPCLIENTDLL_EXPORT void ClientTerminate();
	TCPIPCLIENTDLL_EXPORT void SetConnectionParams(const char *, const char *, const char *);
	TCPIPCLIENTDLL_EXPORT EClientStatus GetClientDllStatus();
}

class MutexLocker
{
    std::mutex *_mutex;
public:
    MutexLocker(std::mutex *mutex)
    {
        _mutex = mutex;
        _mutex->lock();
    }
    ~MutexLocker()
    {
        _mutex->unlock();
    }
};

template <typename T>
class ThreadSafe
{
    std::mutex _mutexRead;
    std::mutex _mutexWrite;
    T value;
public:
    ThreadSafe(const T& other)
    {
        MutexLocker ml1(&_mutexRead);
        MutexLocker ml2(&_mutexWrite);
        value = other;
    }
    ThreadSafe& operator=(const T& other)
    {
        MutexLocker ml1(&_mutexRead);
        MutexLocker ml2(&_mutexWrite);
        value = other;
        return *this;
    }
    operator T()
    {
        MutexLocker ml(&_mutexWrite);
        return value;
    }
};
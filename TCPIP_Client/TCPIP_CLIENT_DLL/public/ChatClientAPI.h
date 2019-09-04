#pragma once
#include <mutex>

#if defined _WIN32 || defined __CYGWIN__
  #ifdef TCPIP_CLIENT_DLL_EXPORTS
    #ifdef __GNUC__
      #define TCPIPCLIENTDLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define TCPIPCLIENTDLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define TCPIPCLIENTDLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define TCPIPCLIENTDLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define TCPIPCLIENTDLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define TCPIPCLIENTDLL_PUBLIC
    #define DLL_LOCAL
  #endif
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
    TCPIPCLIENTDLL_PUBLIC void setCallbackMessageReceived(callbackMessageReceivedFunc);
    TCPIPCLIENTDLL_PUBLIC void ClientMainLoop();
    TCPIPCLIENTDLL_PUBLIC void ClientSendMessage(const char* szTargetName, const int status, const void * data, const int dataLen);
    TCPIPCLIENTDLL_PUBLIC void ClientTerminate();
    TCPIPCLIENTDLL_PUBLIC void SetConnectionParams(const char *, const char *, const char *);
    TCPIPCLIENTDLL_PUBLIC EClientStatus GetClientDllStatus();
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
    ThreadSafe(){};
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

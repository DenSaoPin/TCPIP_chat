#pragma once

#include <ProtocolAPI/Protocol.h>
#include <string>
#include <queue>
#include <mutex>

#define MAX_PORT_DIGIT 5

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

	class TCPIP_Client
	{
	private:
		TCPIP_Client() {};
		~TCPIP_Client() {};
		TCPIP_Client(const TCPIP_Client&);                 // Prevent copy-construction
		TCPIP_Client& operator=(const TCPIP_Client&);

		static TCPIP_Client* _instance;
		std::queue<std::string> m_outgoingMessages;

		bool m_NeedTerminate = false;
		ThreadSafe <bool> m_IsTerminate = false;
		ThreadSafe <bool> m_IsStarted = false;

	public:

		std::string Name;
		std::string ServerIP;
		std::string ServerPort;

		int Socket = NULL;

		static TCPIP_Client* Instance();

		void Initialize(const std::string& name, const std::string& serverIP, const std::string& serverPort);

		ChatLib::Response IntroduceToServer();

		void InitializeSocketRoutine();

		int GetSocket();

		bool GetStatus();

		void ClientMainLoop();
		void SendTextMessage(const char* sz_str);
		void Shutdown();

		//const char* ClientSendMessage(const char *);
};

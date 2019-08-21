#pragma once

#include <string>
#include <queue>
#include <mutex>

#include "ProtocolAPI/BaseMessage.h"
#include "ProtocolAPI/Response.h"
#include "Defines.h"

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

	enum EStatus
	{
		eInvalid = 0,
		eStartWSA,
		eInitializeSocket,
		eIntroduce,
		eReceiveMessage,
		eSendingMessage,
		eShutdown,
	};

	class TCPIP_Client
	{
	private:
		TCPIP_Client() {};
		~TCPIP_Client() {};
		TCPIP_Client(const TCPIP_Client&);                 // Prevent copy-construction
		TCPIP_Client& operator=(const TCPIP_Client&);

		static TCPIP_Client* _instance;

		std::queue<ChatLib::BaseMessagePtr> m_outgoingMessages;
		std::queue<ChatLib::BaseMessagePtr> m_incomingMessages;

		unsigned short m_currentMessageId = 0;

		bool m_NeedTerminate = false;
		ThreadSafe <bool> m_IsTerminated = false;
		ThreadSafe <bool> m_IsStarted = false;

		EStatus m_ClientStatus = eStartWSA;
	public:

		std::string Name;
		std::string ServerIP;
		std::string ServerPort;

		int Socket = NULL;

		static TCPIP_Client* Instance();

		void Initialize(const std::string& name, const std::string& serverIP, const std::string& serverPort);

		bool InitializeSocketRoutine();
		bool InitWinSockDll();

		void ShowError(const std::string &);

		int GetSocket();

		bool GetStatus();
		void ClientMainLoop();
		void Shutdown();

		ChatLib::RawBytes TCPIP_Client::RecieveMessage(const CROSS_SOCKET& socket);

		bool TCPIP_Client::SendMessagee(ChatLib::BaseMessagePtr message, const CROSS_SOCKET& socket);

		void TCPIP_Client::AddForSend(const char* sz_target_name, const int status, const void* data, const int data_len);

		unsigned short GenerateId();
	};

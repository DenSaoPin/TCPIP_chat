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
		eAwaitResponce,
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
		std::queue<std::string> m_outgoingMessages;

		bool m_NeedTerminate = false;
		ThreadSafe <bool> m_IsTerminate = false;
		ThreadSafe <bool> m_IsStarted = false;

		EStatus m_ClientStatus = eInvalid;

	public:

		std::string Name;
		std::string ServerIP;
		std::string ServerPort;

		int Socket = NULL;

		static TCPIP_Client* Instance();

		void Initialize(const std::string& name, const std::string& serverIP, const std::string& serverPort);

		ChatLib::Response NEW_IntroduceToServer();
		ChatLib::Response IntroduceToServer();

		bool InitializeSocketRoutine();
		bool TrySocketConnect();
		bool InitWinSockDll();

		void ShowError(const std::string &);

		int GetSocket();

		bool GetStatus();
		void ClientMain();
		void ClientMainLoop();
		void SendTextMessage(const char* sz_str);
		void Shutdown();

		ChatLib::RawBytes TCPIP_Client::RecieveMessageAndReply(const CROSS_SOCKET& socket);
		ChatLib::RawBytes TCPIP_Client::RecieveMessage(const CROSS_SOCKET& socket);

		ChatLib::Response TCPIP_Client::TrySendMessage(ChatLib::BaseMessage* message, const CROSS_SOCKET& socket);
		void TCPIP_Client::SendMessagee(ChatLib::BaseMessage* message, const CROSS_SOCKET& socket);

		void TCPIP_Client::SendResponse(ChatLib::ResponseStatus status, const CROSS_SOCKET& socket);
};

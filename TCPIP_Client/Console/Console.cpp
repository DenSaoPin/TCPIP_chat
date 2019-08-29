#pragma once

#include <Windows.h>
#include <public/ChatClientAPI.h>
#include <thread>
#include <iostream>
#include <string>
#include "ProtocolAPI/BroadcastMessage.h"
#include <future>
#include <chrono>

enum E_UI_Command
{
    eDefault             = 0,
    eReconnect           = 1,
    eShutdown            = 2,
    eBroadcastMessage    = 3,
    eDirectMessage       = 4,
    eExit                = 5,
};

void OnRecieveMessage(const char *szName, const int* messageType, const char *szMessage);
void OnExit();

std::thread chatThread;
std::thread statusThread;

//std::thread m_clientDllMainThread;
std::future<void> m_clientDllMainThread;
//std::future<void> m_clientDllStatusThread;
std::thread m_clientDllStatusThread;

ThreadSafe<EClientStatus> m_clientStatus = eInvalid;

int UI_status = 0;

using Timer = std::chrono::steady_clock;
using int_sec = std::chrono::duration<int>;
using time_point = std::chrono::time_point<Timer>;

void DllStatusChecker();

void OnExit()
{
	ClientTerminate();
	chatThread.join();
	statusThread.join();
}

BOOL WINAPI ConsoleHandler(DWORD CEvent)
{
	switch (CEvent)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		OnExit();
		break;
	}
	return TRUE;
}

E_UI_Command ParseCommand(const std::string& message, std::string& outTargetName)
{
    E_UI_Command command = eBroadcastMessage;
    int startIndex = message.find("for @");
    if (startIndex != std::string::npos)
    {
        startIndex += 5;
        int finishIndex = message.find("@", startIndex);
        outTargetName = message.substr(startIndex, finishIndex - startIndex);
        command = E_UI_Command::eDirectMessage;
    }
    startIndex = message.find("@reconnect@");
    if (startIndex != std::string::npos)
    {
        command = eReconnect;
    }
    startIndex = message.find("@exit@");
    if (startIndex != std::string::npos)
    {
        command = eExit;
    }
    startIndex = message.find("@shutdown@");
    if (startIndex != std::string::npos)
    {
        command = eShutdown;
    }
    return command;
}

void AwaitUserInput()
{
    std::string message;
    std::string targetName;
    while (true)
    {
        std::getline(std::cin, message);

        if (message.empty())
            continue;

        E_UI_Command command = ParseCommand(message, targetName);
        switch (command)
        {
        case E_UI_Command::eDirectMessage:
        {
            std::string szStr = message.c_str();
            ClientSendMessage(nullptr, static_cast<const int>(ChatLib::eBroadcastMessage), reinterpret_cast<const void*>(message.c_str()), szStr.size());
            break;
        }
        case E_UI_Command::eBroadcastMessage:
        {
            std::string szStr = message.c_str();
            ClientSendMessage(nullptr, static_cast<const int>(ChatLib::eBroadcastMessage), reinterpret_cast<const void*>(message.c_str()), szStr.size());
            break;
        }
        case E_UI_Command::eReconnect:
        {
            int_sec sleepTime{ 0 };
            const std::future_status status = m_clientDllMainThread.wait_for(sleepTime);

            //TODO how to check valid async thread or not? m_clientDllMainThread.valid();
            if (m_clientStatus == EClientStatus::eTerminating)
            {
            }
            else if (m_clientStatus == EClientStatus::eShutDown)
            {
            }
            else
            {
                ClientTerminate();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            m_clientDllMainThread = std::async(std::launch::async, ClientMainLoop);
        }
            break;
        case E_UI_Command::eShutdown:
            ClientTerminate();
            break;
        case E_UI_Command::eExit:
            exit(0);
            break;
        case E_UI_Command::eDefault:
            throw new std::exception(" Default command type in ConsoleUI \n");
            break;
        default:
            throw new std::exception(" Unknown command type in ConsoleUI \n");
            break;
        }
    }
}

void DllStatusChecker()
{
	EClientStatus status =  EClientStatus::eInvalid;

    time_point lapStart = Timer::now();
    const int_sec timeGap{1};
    while (true)
	{
        if(Timer::now() > lapStart + timeGap)
        {
            status = GetClientDllStatus();

            m_clientStatus = status;

            if (status == EClientStatus::eShutDown)
            {
                std::cout << " You are disconnected, please try @reconnect@, if you @exit@ some of your messages can be undelivered \n";
                //AwaitUserInput();
                //break;
            }
            lapStart = Timer::now();
        }
	}
}

class ClientSettings
{
public:
	std::string Name = "";
	std::string Adress = "";
	std::string Port = "";

	void SetParams()
	{
		std::string name;
		std::cin >> name;

		Name = name;
		Adress = "127.0.0.1";
		Port = "7700";

		SetConnectionParams(Name.c_str(), Adress.c_str(), Port.c_str());
	}
};
int main()
{
    m_clientDllStatusThread = std::thread(DllStatusChecker);

	ClientSettings settings;
	settings.SetParams();

	SetConsoleCtrlHandler(ConsoleHandler, TRUE);

	setCallbackMessageReceived(OnRecieveMessage);

    //statusThread = std::thread(DllStatusChecker);
	m_clientDllMainThread = std::async(std::launch::async, ClientMainLoop);

    AwaitUserInput();
}

	void OnRecieveMessage(const char *szName, const int* messageType, const char *szMessage)
	{
		printf("%s: %s" "\n", szName, szMessage);
	}
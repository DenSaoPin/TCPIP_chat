// Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <iostream>
#include  "ChatClientAPI.h"

void OnRecieveMessage(const char *szMessage);

int main()
{
	setCallbackMessageReceived(OnRecieveMessage);

	//DllMainLoop();
	//while(1)
	//{
	//	std::string message;
	//	std::cin >> message;
	//	SendMessage(message.c_str())
	//}
}


void OnRecieveMessage(const char *szMessage)
{
	printf("%s", szMessage);
}

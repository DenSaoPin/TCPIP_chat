#pragma once
enum  PackageType
{
	eRequest = 0x01,
	eResponce = 0x02,
};

enum Request
{
	eSendName = 0x01, 
	eSendMessage = 0x02,
};

enum Response
{
	eOk = 0x01,
	eError = 0x02,
};
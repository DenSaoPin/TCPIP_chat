#pragma once

namespace ChatLib
{
	//enum  PackageType
	//{
	//	eRequest = 0x01,
	//	eResponce = 0x02,
	//};

	//enum Request
	//{
	//	eSendName = 0x01,
	//	eSendMessage = 0x02,
	//};

	//enum Response
	//{
	//	eOk = 0x01,
	//	eError = 0x02,
	//	eNotSet = 0x03,
	//};
	enum MessageType
	{
		eNotSet = 0x00,
		eNameRequest = 0x01,
		eMessageRequest = 0x02,
		eResponseOk = 0x03,
		eResponceError = 0x04,
	};
}

#pragma once
#include <string>
#include "Enums.h"
#include "Protocol.h"



namespace ChatLib
{

	class Message//:std::string
	{
	private:
		std::string MessageText;
		std::string Header;

		char _mStartHeaderPoint[4] = { (char)0xFF, (char)0xFF, (char)0xFF, (char)0xFF };
		//PackageType _mPackageType;
		//Response _mResponse;
		//Request _mRequest;

		//void FillHeader(PackageType type, Request requestVal)
		//{
		//	Header.append(_mStartHeaderPoint, 4);
		//	Header.push_back((char)eResponce);
		//	Header.push_back((char)requestVal);
		//}
		//void FillHeader(PackageType type, Response responseVal)
		//{
		//	Header.append(_mStartHeaderPoint, 4);
		//	Header.push_back((char)eResponce);
		//	Header.push_back((char)responseVal);
		//}
		void FillHeader(MessageType messageType)
		{
			Header.append(_mStartHeaderPoint, 4);
			Header.push_back((char)messageType);
		}
		void FillHeader()
		{
			Header.append(_mStartHeaderPoint, 4);
		}


	public:
		//std::string MessageID;

		Message(MessageType messageType, std::string *pstrMessage = nullptr)
		{
			FillHeader(messageType);
			if (pstrMessage != nullptr)
			{
				MessageText = *pstrMessage;
				Header.push_back((char)MessageText.length());
				//_package = Header + MessageText;
			}
		}


		//Message(Response responseVal, std::string *pstrMessage = nullptr)
		//{
		//	FillHeader(eResponce, responseVal);
		//	if (pstrMessage != nullptr)
		//	{
		//		MessageText = *pstrMessage;
		//		Header.push_back(MessageText.length());
		//		//_package = Header + MessageText;
		//	}
		//}

		//Message(Request requestVal, std::string message)
		//{
		//	FillHeader(eRequest, requestVal);

		//	MessageText = message;
		//	Header.push_back(message.length());
		//}

		Message(std::string str)
		{
			FillHeader();
			//TODO check copy or ref
			MessageText = str;
			Header.push_back((char)MessageText.length());
		}

		Message(MessageType messageType, char* buff)
		{
			FillHeader(messageType);
			//TODO check 
			//MessageText.append(buff, (int)buff[5]);

			if(messageType == eMessageRequest || messageType == eNameRequest)
			{
				MessageText.append(buff, MESSAGE_START_TEXT_INDEX, (int)buff[MESSAGE_LENGTH_INDEX]);
			}

			Header.push_back((char)MessageText.length());
		}

		//Message(std::string str)
		//{
		//	//TODO check copy or ref
		//	MessageText = str;
		//}
		Message()
		{
			FillHeader();

			Header.push_back((char)0);
		}

		//int GetPackageLength()
		//{
		//	return _package.length();
		//}
		//TODO need references?
		std::string GetHeader()
		{
			return  Header;
		}
		void SetHeader(const std::string header)
		{
			Header = header;
		}
		void SetHeader()
		{
			FillHeader();
		}

		std::string GetText()
		{
			return  MessageText;
		}
		void SetText(const std::string text)
		{
			MessageText = text;
		}

		MessageType GetType()
		{
			return  (MessageType)Header[MESSAGE_TYPE_INDEX];
		}
		int GetTextLength()
		{
			 return MessageText.length();
		}

		//std::string GetFullPackage()
		//{
		//	CombineFullPackage();

		//	return _package;
		//}
	};
	typedef Message & MessageR;
	typedef Message * MessageP;
}

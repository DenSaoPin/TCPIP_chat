#pragma once
#include <string>
#include "Enums.h"
#include "Protocol.h"
#include <xlocmon>


namespace ChatLib
{

	class Message//:std::string
	{
		

	public:
		class Header
		{
		public:
			char MessageType = 0;
			char MessageLength = 0;

			Header (const ChatLib::MessageType& messageType, const int& messageLength = 0)
			{
				MessageType = messageType;
				MessageLength = messageLength;
			}

			Header()
			{
			}

			int WriteBytes(char *pBuffer) const
			{
				int b = 0;

				pBuffer[b++] = (char)0xFF;
				pBuffer[b++] = (char)0xFF;
				pBuffer[b++] = (char)0xFF;
				pBuffer[b++] = (char)0xFF;
				pBuffer[b++] = MessageType;
				pBuffer[b++] = MessageLength;

				return b;
			}
			//TODO where is good place?
			//const std::string operator+(const Header& left, const std::string &str)
			//{
			//	return (left._fullHeaderText + str);
			//}

		};

	private:
		std::string messageText;
		Header header;
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

		//void FillHeader(MessageType messageType)
		//{
		//	Header.MessageType = messageType;
		//}
		//void FillHeader()
		//{
		//}


	public:
		//std::string MessageID;

		//TODO we will crush is string is nullptr or not?
		//TODO we can create two constructors
		Message(const MessageType& messageType, const std::string& strMessage) 
			: header(messageType)
		{
			SetText(strMessage);
		}

		Message(const MessageType& messageType) 
			: header(messageType)
		{
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

		//TODO check where we use it 
		//Message(std::string str) : Header()
		//{
		//	FillHeader();
		//	//TODO check copy or ref
		//	MessageText = str;
		//	Header.push_back((char)MessageText.length());
		//}

		//TODO check crush or not with string
		Message(const MessageType& messageType, char* buff) 
			: header(messageType, (int)buff[MESSAGE_LENGTH_INDEX])
		{
			if(header.MessageLength > 0)
				messageText = std::string(buff + MESSAGE_START_TEXT_INDEX, buff[MESSAGE_LENGTH_INDEX]);

			header.MessageLength = (char)messageText.length();
		}

		//Message(std::string str)
		//{
		//	//TODO check copy or ref
		//	MessageText = str;
		//}



		//int GetPackageLength()
		//{
		//	return _package.length();
		//}
		//TODO need references?

		const Header & GetHeader() const
		{
			return  header;
		}

		int WriteBytes(char *pBuffer) const
		{
			int bytes = header.WriteBytes(pBuffer);
			pBuffer += bytes;
			//TODO after .cstr we have changed strnig?
			memcpy(pBuffer, messageText.c_str(), messageText.length());
			bytes += (int)messageText.length();
			return bytes;
		}

		/*
		std::string GetHeaderLikeText()
		{
			return  header.GetHeaderLikeText();
		}
		*/


		//TODO check where we use
		//void SetHeader(const std::string header)
		//{
		//	this->header = header;
		//}

		//TODO check where we use
		//void SetHeader()
		//{
		//	FillHeader();
		//}

		const std::string const GetText() const
		{
			return messageText;
		}
		void SetText(const std::string & text)
		{
			messageText = text;
			header.MessageLength = (char)text.length();
			//TODO if we use thay we must set header.TextLength
		}

		//TODO what will do if MessageType is not valid 
		const MessageType GetType() const
		{
			return  (MessageType)header.MessageType;
		}

		const int GetTextLength() const
		{
			 return (int)messageText.length();
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

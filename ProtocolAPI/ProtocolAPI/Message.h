#pragma once
#include <string>
#include "Enums.h"
#include "Protocol.h"


namespace ChatLib
{

	class Message
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
		};

	private:
		std::string messageText;
		Header header;

	public:
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

		//TODO check crush or not with string
		Message(const MessageType& messageType, char* buff) 
			: header(messageType, (int)buff[MESSAGE_LENGTH_INDEX])
		{
			if(header.MessageLength > 0)
				messageText = std::string(buff + MESSAGE_START_TEXT_INDEX, buff[MESSAGE_LENGTH_INDEX]);

			header.MessageLength = (char)messageText.length();
		}

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
	};
	typedef Message & MessageR;
	typedef Message * MessageP;
}

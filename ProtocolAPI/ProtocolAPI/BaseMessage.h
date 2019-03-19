#pragma once
#include "stdafx.h"
#include <vector>
#include "Enums.h"
#include <memory>

namespace ChatLib
{
	typedef unsigned char byte;
	typedef byte * byteP;
	typedef std::vector<byte> RawBytes;

#define HEADER_SIZE 6
#define MAGIC_NUMBER_LENGTH 4

#define MESSAGE_LENGTH_INDEX 5
#define MESSAGE_TYPE_INDEX 4

	class BaseMessage
	{
#define HEADER_START 0xFFFFFFFF
#define MESSAGE_TYPE_INDEX 4

	protected:
		unsigned int _magicNumber = HEADER_START;
		byte _messageType = 0;

	public:
		BaseMessage(const RawBytes& rawData);

		BaseMessage(const MessageType& messageType);

		BaseMessage(byteP &pBuffer);
	public:
		static MessageType GetType(const byte *pBuff);

		static MessageType GetType(RawBytes& rawData);
	public:
		virtual int Construct(byte* pBuff);

		virtual ~BaseMessage();
	};

	typedef std::shared_ptr<BaseMessage> BaseMessagePtr;
}

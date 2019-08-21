#pragma once
#include <vector>
#include "Enums.h"
#include <memory>

namespace ChatLib
{
	typedef unsigned char byte;
	typedef byte * byteP;
	typedef std::vector<byte> RawBytes;

#define HEADER_SIZE 8
#define MAGIC_NUMBER_LENGTH 4

#define MESSAGE_LENGTH_INDEX 7
#define MESSAGE_TYPE_INDEX 6

#define MESSAGE_ID_INDEX 4
#define MESSAGE_ID_LENGTH 2
	
	class BaseMessage
	{
#define HEADER_START 0xFFFFFFFF

	protected:
		unsigned int _magicNumber = HEADER_START;
		byte _messageType = 0;
		unsigned short _id = 0;

	public:
		BaseMessage(const RawBytes& rawData);

		BaseMessage(const MessageType& messageType, const unsigned short& id);

		BaseMessage(byteP &pBuffer);
	public:
		static MessageType GetType(const byte *pBuff);

		static MessageType GetType(RawBytes& rawData);

		MessageType GetMyType() const;

		unsigned short GetMyID() const;

	public:
		virtual int Construct(byte* pBuff);

		virtual std::string GetDebugString();

		virtual ~BaseMessage();

	};

	typedef std::shared_ptr<BaseMessage> BaseMessagePtr;
}

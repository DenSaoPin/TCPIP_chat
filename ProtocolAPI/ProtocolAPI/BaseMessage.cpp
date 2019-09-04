#include "BaseMessage.h"

namespace ChatLib 
{
	MessageType BaseMessage::GetType(const ChatLib::byte *pBuff)
	{
		if (*((int*)pBuff) == HEADER_START)
		{
			char messageType = pBuff[MESSAGE_TYPE_INDEX];

			if (messageType >= ChatLib::eInvalid && messageType <= ChatLib::eResponse)
				return (ChatLib::MessageType)messageType;

            throw std::runtime_error(" Not filled package type or package is trash \n");
		}
        throw std::runtime_error(" It isn`t message \n");
	}
	MessageType BaseMessage::GetType(ChatLib::RawBytes& rawData)
	{
		return GetType(&(rawData[0]));
	}

	MessageType BaseMessage::GetMyType() const
	{
		return (ChatLib::MessageType)_messageType;
	}

	unsigned short BaseMessage::GetMyID() const
	{
		return _id;
	}

	int BaseMessage::Construct(ChatLib::byte* pBuff)
	{
		int count = 0;
		pBuff[count++] = 0xFF;
		pBuff[count++] = 0xFF;
		pBuff[count++] = 0xFF;
		pBuff[count++] = 0xFF;
		*((unsigned short *)&(pBuff[count])) = this->_id;
		count++;
		count++;
		pBuff[count++] = this->_messageType;
		return count;
	}

	std::string BaseMessage::GetDebugString()
	{
		return std::string ("It is BaseMessage");
	}

	BaseMessage::~BaseMessage()
	{
	}

	BaseMessage::BaseMessage(const ChatLib::RawBytes& rawData)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (rawData[i] != 0xFF)
                throw std::runtime_error("BaseMesage constructor error: string argue don`t start with magic number");
		}
		if (rawData[MESSAGE_TYPE_INDEX] < ChatLib::eInvalid || rawData[MESSAGE_TYPE_INDEX] > ChatLib::eResponse)
            throw std::runtime_error("BaseMesage constructor error: message type not recognized");

		_id = *(unsigned short *)rawData[MESSAGE_ID_INDEX];
		_messageType = rawData[MESSAGE_TYPE_INDEX];
	}

	BaseMessage::BaseMessage(const ChatLib::MessageType& messageType, const unsigned short& id)
	{
		if (messageType < ChatLib::eInvalid || messageType > ChatLib::eResponse)
            throw std::runtime_error("BaseMesage constructor error: message type not recognized");
		_id = id;
		_messageType = messageType;
	}

	BaseMessage::BaseMessage(byteP & pBuffer)
	{
		//TODO check!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (*(unsigned int*)(pBuffer) == HEADER_START)
		{
			pBuffer += MAGIC_NUMBER_LENGTH;

			_id = *((unsigned short*)pBuffer);
			pBuffer += MESSAGE_ID_LENGTH;


			if (*pBuffer < ChatLib::eInvalid || *pBuffer > ChatLib::eResponse)
                throw std::runtime_error("BaseMesage constructor error: message type not recognized");

			_messageType = *(pBuffer++);
		}
	}
}

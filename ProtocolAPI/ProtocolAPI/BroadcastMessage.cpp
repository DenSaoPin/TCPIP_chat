#include "BroadcastMessage.h"
#include <ostream>

namespace  ChatLib
{

	int BroadcastMessage::Construct(byte* pBuff) 
	{
		//TODO test!!!!!!!!!!!!!!!!!
		int count = BaseMessage::Construct(pBuff);

		int strSize = SourceName.size();
		pBuff[count++] = strSize;
		memcpy((char *)pBuff + count, this->SourceName.c_str(), strSize);
		count += strSize;

		strSize = Text.size();
		pBuff[count++] = strSize;
		memcpy((char *)pBuff + count, this->Text.c_str(), strSize);

		pBuff += count;
		count += strSize;

		return count;
	}

	std::string BroadcastMessage::GetDebugString()
	{
		return std::string("It is BroadcastMessage. Text = " + Text + "\n");
	}

	BroadcastMessage::BroadcastMessage(std::string& sourceName, std::string& text, const unsigned short& id) : BaseMessage(ChatLib::eBroadcastMessage, id)
	{
		Text = text;
		SourceName = sourceName;
	}

	BroadcastMessage::BroadcastMessage(byteP& pBuff) : BaseMessage(pBuff)
	{
		const int sourceNameLength = *(pBuff++);

		if (sourceNameLength > 0)
			SourceName = std::string((char*)pBuff, sourceNameLength);

		pBuff += sourceNameLength;

		//TODO check !!!!!!!!!!! change pointer or value
		const byte messageLength = *(pBuff++);

		if (messageLength > 0)
			Text = std::string((char *)pBuff, messageLength);
		pBuff += messageLength;
	}
}


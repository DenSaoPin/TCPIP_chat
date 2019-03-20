#include "stdafx.h"
#include "BroadcastMessage.h"
#include <ostream>

namespace  ChatLib
{

	int BroadcastMessage::Construct(byte* pBuff) 
	{
		//TODO test!!!!!!!!!!!!!!!!!
		int len = BaseMessage::Construct(pBuff);
		pBuff[len++] = Text.size();
		memcpy((char *)pBuff + len, this->Text.c_str(), Text.size());
		len += Text.size();
		return len;
	}

	std::string BroadcastMessage::GetDebugString()
	{
		return std::string("It is BroadcastMessage. Text = " + Text + "\n");
	}

	BroadcastMessage::BroadcastMessage(std::string& text): BaseMessage(ChatLib::eBroadcastMessage)
	{
		Text = text;
	}

	BroadcastMessage::BroadcastMessage(byteP& pBuff) : BaseMessage(pBuff)
	{
		//TODO check !!!!!!!!!!! change pointer or value
		const byte messageLength = *(pBuff++);

		if (messageLength > 0)
			Text = std::string((char *)pBuff, messageLength);
		pBuff += messageLength;
	}
}


#include "stdafx.h"
#include "BroadcastMessage.h"

namespace  ChatLib
{

	int BroadcastMessage::Construct(ChatLib::byte* pBuff) 
	{
		//TODO test!!!!!!!!!!!!!!!!!
		int len = BaseMessage::Construct(pBuff);
		pBuff[len++] = Text.size();
		memcpy((char *)pBuff + len, this->Text.c_str(), Text.size());
		len += Text.size();
		return len;
	}

	BroadcastMessage::BroadcastMessage(std::string& text): BaseMessage(ChatLib::eBroadcastMessage)
	{
		Text = text;
	}

	BroadcastMessage::BroadcastMessage(byteP& pBuff) : BaseMessage(pBuff)
	{
		//TODO check !!!!!!!!!!! change pointer or value
		ChatLib::byte messageLength = *(pBuff++);

		if (messageLength > 0)
			Text = std::string((char *)pBuff, messageLength);
		pBuff += messageLength;
	}
}


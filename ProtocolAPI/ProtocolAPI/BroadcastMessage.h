#pragma once
#include "BaseMessage.h"
namespace ChatLib
{
	class BroadcastMessage :
		public BaseMessage
	{
#define BROADCAST_MESSAGE_START_TEXT_INDEX 6

	public:
		std::string Text;
	public:
		BroadcastMessage(byteP &pBuff);

		BroadcastMessage(std::string& text);
		//~BroadcastMessage();

		int Construct(ChatLib::byte* pBuff) override;
	};
}
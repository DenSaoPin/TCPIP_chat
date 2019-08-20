#pragma once
#include "BroadcastMessage.h"
namespace ChatLib
{
	class NameRequestMessage : public BroadcastMessage
	{
	public:
		NameRequestMessage(byteP &ppBuff);

		NameRequestMessage(std::string& name);

		std::string GetDebugString() override;
	};

	typedef std::shared_ptr<NameRequestMessage> NameRequestMessagePtr;
}


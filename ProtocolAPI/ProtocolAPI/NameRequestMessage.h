#pragma once
#include "BroadcastMessage.h"
namespace ChatLib
{
	class NameRequestMessage : public BroadcastMessage
	{
	public:
		NameRequestMessage(byteP &ppBuff);

		NameRequestMessage(std::string& name, const unsigned short & id);

		std::string GetDebugString() override;
	};

	typedef std::shared_ptr<NameRequestMessage> NameRequestMessagePtr;
}


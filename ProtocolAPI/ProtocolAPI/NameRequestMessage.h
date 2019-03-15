#pragma once
#include "BroadcastMessage.h"
namespace ChatLib
{
	class NameRequestMessage : public BroadcastMessage
	{
	public:
		NameRequestMessage(byteP &ppBuff);

		NameRequestMessage(std::string& name);
	};
}


#include "stdafx.h"
#include "NameRequestMessage.h"

ChatLib::NameRequestMessage::NameRequestMessage(byteP &ppBuff): BroadcastMessage(ppBuff)
{
}

ChatLib::NameRequestMessage::NameRequestMessage(std::string& name): BroadcastMessage(name)
{
	_messageType = eNameRequest;
}

#include "NameRequestMessage.h"

ChatLib::NameRequestMessage::NameRequestMessage(byteP &ppBuff): BroadcastMessage(ppBuff)
{
}

ChatLib::NameRequestMessage::NameRequestMessage(std::string& name): BroadcastMessage(name)
{
	_messageType = eNameRequest;
}

std::string ChatLib::NameRequestMessage::GetDebugString()
{
	return std::string("It is NameRequest");
}

#include "NameRequestMessage.h"

ChatLib::NameRequestMessage::NameRequestMessage(byteP &ppBuff): BroadcastMessage(ppBuff)
{
}

ChatLib::NameRequestMessage::NameRequestMessage(std::string& name, const unsigned short & id): BroadcastMessage(name, name, id)
{
	_messageType = eNameRequest;
}

std::string ChatLib::NameRequestMessage::GetDebugString()
{
	return std::string("It is NameRequest");
}

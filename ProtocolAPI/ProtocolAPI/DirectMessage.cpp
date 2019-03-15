#include "stdafx.h"
#include "DirectMessage.h"


ChatLib::DirectMessage::DirectMessage(byteP &pBuff): BaseMessage(pBuff)
{
	//TODO check !!!!!!!!!!! change pointer or value
	int messageLength = *(pBuff++);

	if (messageLength > 0)
		Text = std::string((char *)*pBuff, messageLength);
}

ChatLib::DirectMessage::DirectMessage(std::string& targetName, std::string& text): BaseMessage(ChatLib::eDirectMessage)
{
	Text = text;
	TargetName = targetName;
}

int ChatLib::DirectMessage::Construct(ChatLib::byte* pBuff)
{
	//TODO test!!!!!!!!!!!!!!!!!
	int count = BaseMessage::Construct(pBuff);

	int strSize = TargetName.size();
	pBuff[count++] = strSize;
	strcpy_s((char *)pBuff + count, strSize, this->TargetName.c_str());
	count += strSize;

	strSize = Text.size();
	pBuff[count++] = strSize;
	strcpy_s((char *)pBuff + count, strSize, this->Text.c_str());
	count += strSize;

	return count;
}

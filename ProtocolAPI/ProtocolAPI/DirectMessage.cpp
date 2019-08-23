#include "DirectMessage.h"


ChatLib::DirectMessage::DirectMessage(byteP &pBuff): BaseMessage(pBuff)
{
	//TODO check !!!!!!!!!!! change pointer or value
	int targetNameLength = *(pBuff++);

	if (targetNameLength > 0)
		TargetName = std::string((char*)pBuff, targetNameLength);

	pBuff += targetNameLength;

	int sourceNameTextLength = *(pBuff++);

	if (sourceNameTextLength > 0)
		SourceName = std::string((char*)pBuff, sourceNameTextLength);

	pBuff += sourceNameTextLength;

	int textLength = *(pBuff++);

	if (textLength > 0)
		Text = std::string((char *)pBuff, textLength);

	pBuff += textLength;
}

ChatLib::DirectMessage::DirectMessage(std::string& sourceName, std::string& targetName, std::string& text, const unsigned short & id): BaseMessage(eDirectMessage, id)
{
	Text = text;
	TargetName = targetName;
	SourceName = sourceName;
}

int ChatLib::DirectMessage::Construct(byte* pBuff)
{
	//TODO test!!!!!!!!!!!!!!!!!
	int count = BaseMessage::Construct(pBuff);

	int strSize = TargetName.size();
	pBuff[count++] = strSize;
	memcpy((char *)pBuff + count, this->TargetName.c_str(), strSize);
	count += strSize;

	strSize = Text.size();
	pBuff[count++] = strSize;
	memcpy((char *)pBuff + count, this->Text.c_str(), strSize);

	pBuff += count;
	count += strSize;

	return count;
}

std::string ChatLib::DirectMessage::GetDebugString()
{
	return std::string("It is DirectMessage. TargetName = " + TargetName + " Text = " + Text + "\n");
}

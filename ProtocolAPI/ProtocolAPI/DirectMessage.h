#pragma once
#include "BaseMessage.h"
namespace ChatLib
{
	class DirectMessage :
		public BaseMessage
	{
	public:
		std::string SourceName;
		std::string Text;
		std::string TargetName;
	public:
		DirectMessage(byteP &pBuff);
		
		DirectMessage(std::string& sourceName, std::string& targetName, std::string& text, const unsigned short& id);

		int Construct(ChatLib::byte* pBuff) override;

		std::string GetDebugString() override;
	};
	typedef std::shared_ptr<DirectMessage> DirectMessagePtr;
}





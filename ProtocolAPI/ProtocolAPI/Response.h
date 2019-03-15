#pragma once
#include "BaseMessage.h"

namespace ChatLib
{
	class Response : public BaseMessage
	{
#define RESPONSE_OK_LENGTH 5
#define RESPONSE_STATUS_INDEX 5

		ChatLib::ResponseStatus _responseStatus = ChatLib::eResponseInvalid;
	public:
		Response(ChatLib::RawBytes rawData);

		Response(byteP & ppBuffer);

		Response(ChatLib::ResponseStatus status);

		int Construct(ChatLib::byte* pBuff) override;

		ResponseStatus GetStatus() const;
	};
}

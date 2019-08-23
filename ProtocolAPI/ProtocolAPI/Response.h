#pragma once
#include "BaseMessage.h"

namespace ChatLib
{
	class Response : public BaseMessage
	{
#define RESPONSE_STATUS_INDEX 7

		ChatLib::ResponseStatus _responseStatus = eResponseInvalid;
	public:
		Response(RawBytes rawData);

		Response(byteP & ppBuffer);

		Response(ResponseStatus status, const unsigned short &id);

		int Construct(byte* pBuff) override;

		ResponseStatus GetStatus() const;

		std::string GetDebugString() override;
	};

	typedef std::shared_ptr<Response> ResponsePtr;
}

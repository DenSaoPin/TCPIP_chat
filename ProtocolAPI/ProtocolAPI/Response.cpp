#include "stdafx.h"
#include "Response.h"

namespace ChatLib
{
	Response::Response(RawBytes rawData) : BaseMessage(rawData)
	{
		_responseStatus = (ResponseStatus)rawData[RESPONSE_STATUS_INDEX];
	}

	Response::Response(byteP &pBuffer) : BaseMessage(pBuffer)
	{
		_responseStatus = (ResponseStatus)*(pBuffer++);
	}

	Response::Response(ResponseStatus status) : BaseMessage(eResponse)
	{
		_responseStatus = status;
	}

	int Response::Construct(byte* pBuff)
	{
		//TODO test!!!!!!!!!!!!!!!!!
		int len = BaseMessage::Construct(pBuff);
		pBuff[len++] = _responseStatus;
		return len;
	}

	ResponseStatus Response::GetStatus() const
	{
		return _responseStatus;
	}
}
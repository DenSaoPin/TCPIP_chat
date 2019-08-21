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

	Response::Response(ResponseStatus status, const unsigned short &id) : BaseMessage(eResponse, id)
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

	std::string Response::GetDebugString()
	{
		std::string status;
		switch (_responseStatus)
		{
		case eInvalid:
			status = "eInvalid";
			break;
		case eOk:
			status = "eOk";
			break;
		case eError:
			status = "eError";
			break;
		case eNameConflict:
			status = "eNameConflict";
			break;
		}

		return std::string(" It is Response. Status = " + status + "\n");
	}
}

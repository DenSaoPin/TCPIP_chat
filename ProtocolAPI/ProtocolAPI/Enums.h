#pragma once

namespace ChatLib
{

	enum MessageType
	{
		eInvalid		= 0x00,
		eNameRequest	= 0x01,
		eMessageRequest = 0x02,
		eResponseOk		= 0x03,
		eResponceError	= 0x04,
		eDirectMessage  = 0x05,
	};
}

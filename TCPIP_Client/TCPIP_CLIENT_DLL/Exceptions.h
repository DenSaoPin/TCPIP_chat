#pragma once
#include <exception>

namespace Exceptions
{
	class ConnectionLostException : public std::exception
	{
	public: ConnectionLostException(const char * text) : exception(text) {}
	};
	class ConnectionClosedException : public std::exception
	{
	public: ConnectionClosedException(const char * text) : exception(text) {}
	};
}

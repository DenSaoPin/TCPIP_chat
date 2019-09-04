#pragma once
#include <exception>

namespace Exceptions
{
    class CommonException : public std::exception
    {
        std::string m_text;

    public:
        CommonException(const std::string inText)
        {
            m_text = inText;
        }

        std::string GetText()
        {
            return m_text;
        }

    };

    class ConnectionLostException : public std::runtime_error
	{
    public: ConnectionLostException(const char * text) : runtime_error(text) {}
	};
    class ConnectionClosedException : public std::runtime_error
	{
    public: ConnectionClosedException(const char * text) : runtime_error(text) {}
    };
}

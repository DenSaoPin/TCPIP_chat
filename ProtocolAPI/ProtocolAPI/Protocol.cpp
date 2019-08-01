#include "Protocol.h"
#include <vector>
#include "Defines.h"
#include "Response.h"
#include "Enums.h"

namespace ChatLib
{	
	RawBytes Protocol::RecieveMessageAndReply(const CROSS_SOCKET& socket)
	{
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(socket, &rfds);
		int maxFD = (int)socket + 1;

		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 500;
		//Message message(eInvalid);
		RawBytes rawData;

		int result = select(maxFD, &rfds, NULL, NULL, &timeout);
		if (result < 0)
		{
			PrintErrors;
			//win errors
			//TODO UNIX check error
		}
		else if (result == 0)
		{ 
			//TODO win check error and timeout
		}
		else
		{
			if (FD_ISSET(socket, &rfds))
			{
				rawData = RecieveMessage(socket);

				//TODO refactor this
				switch (BaseMessage::GetType(rawData))
				{
				case eInvalid:
				case eResponse:
					break;
				default:
					SendResponse(ResponseStatus::eOk, socket);
					break;
				}
			}
		}
		return rawData;
	}

	RawBytes Protocol::RecieveMessage(const CROSS_SOCKET& socket)
	{
		//TODO check recieved num
		char buff[MAX_PACKAGE_LENGTH];
		int recived = recv(socket, buff, MAX_PACKAGE_LENGTH, NULL);
		printf("RecieveMessage %d bytes: ", recived);

		//TODO check raw data
		RawBytes rawData(buff, buff + recived);
		for (int i = 0; i < recived; i++)
		{
			printf("%02X ", buff[i]);
		}
		printf("\n");
		if (recived == 0)
		{
			printf("Recieve message: connection softly closed");
			throw ConnectionClosedException("connection softly closed");
			//TODO error check Win and Timeout
		}
		else if (recived == -1)
		{
			printWsaError();
			closesocket(socket);
			throw ConnectionLostException("connection lost closed");
			//WSACleanup();
			//TODO check Unix error
		}

			printf("Message was recieved \n");

			//MessageType type = BaseMessage::GetType(buff);
			
			//printf("Recieved message type equal %d \n", type);

			////Message message (type, buff);

			//printf("Recieved message equal %s \n", message.GetText());
			
			return rawData;
	}

	Response Protocol::TrySendMessage(BaseMessage* message, const CROSS_SOCKET& socket)
	{
		//TODO need to know sent message or not
		SendMessagee(message, socket);

		auto rawResponseData = RecieveMessage(socket);

		MessageType type = BaseMessage::GetType(rawResponseData);

		if (type != eResponse)
			throw std::exception("I received not response after sending");

		return Response(rawResponseData);
	}
	void Protocol::SendMessagee(BaseMessage* message, const CROSS_SOCKET& socket)
	{
		byte pBuffer[255];
		int length = message->Construct(pBuffer);

		int sended = 0;
		do
		{
			sended = send(socket, (char *)&pBuffer[sended], length, NULL);
			length -= sended;
		} while (length > 0);

	}

	void Protocol::SendResponse(ResponseStatus status, const CROSS_SOCKET& socket)
	{
		Response response(status);
		SendMessagee(&response, socket);
	}

	const bool Protocol::IsLegalPackage(const char* buff)
	{
		return false;
	}

	//const MessageType Protocol::GetMessageType(const char* buff)
	//{
	//	if (*((int*)buff) == HEADER_START)
	//	{
	//		char messageType = buff[MESSAGE_TYPE_INDEX];

	//		if (messageType >= eInvalid && messageType <= eResponse)
	//			return (MessageType)messageType;

	//		throw std::exception(" Not filled package type or package is trash \n");
	//	}
	//	throw std::exception(" It isn`t message \n");
	//}

	void Protocol::printWsaError()
	{
		switch (WSAGetLastError())
		{
		case WSANOTINITIALISED: printf("WSANOTINITIALISED: A successful WSAStartup call must occur before using this function.\n"); break;

		case WSAENETDOWN: printf("WSAENETDOWN: The network subsystem has failed.\n"); break;

		case WSAEFAULT: printf("WSAEFAULT: The buf parameter is not completely contained in a valid part of the user address space.\n"); break;

		case WSAENOTCONN: printf("WSAENOTCONN: The socket is not connected.\n"); break;

		case WSAEINTR: printf("WSAEINTR: The (blocking) call was canceled through WSACancelBlockingCall.\n"); break;

		case WSAEINPROGRESS: printf("WSAEINPROGRESS: A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.\n"); break;

		case WSAENETRESET: printf("WSAENETRESET: For a connection-oriented socket, this error indicates that the connection has been broken due to keep-alive activity that detected a failure while the operation was in progress. For a datagram socket, this error indicates that the time to live has expired.\n"); break;

		case WSAENOTSOCK: printf("WSAENOTSOCK: The descriptor is not a socket.\n"); break;

		case WSAEOPNOTSUPP: printf("WSAEOPNOTSUPP: MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations.\n"); break;

		case WSAESHUTDOWN: printf("WSAESHUTDOWN: The socket has been shut down; it is not possible to receive on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH.\n"); break;

		case WSAEWOULDBLOCK: printf("WSAEWOULDBLOCK: The socket is marked as nonblocking and the receive operation would block.\n"); break;

		case WSAEMSGSIZE: printf("WSAEMSGSIZE: The message was too large to fit into the specified buffer and was truncated.\n"); break;

		case WSAEINVAL: printf("WSAEINVAL: The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative.\n"); break;

		case WSAECONNABORTED: printf("WSAECONNABORTED: The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.\n"); break;

		case WSAETIMEDOUT: printf("WSAETIMEDOUT: The connection has been dropped because of a network failure or because the peer system failed to respond.\n"); break;

		case WSAECONNRESET: printf("WSAECONNRESET: The virtual circuit was reset by the remote side executing a hard or abortive close. The application should close the socket as it is no longer usable. On a UDP-datagram socket, this error would indicate that a previous send operation resulted in an ICMP \"Port Unreachable\" message.\n"); break;
		}
	}

	void Protocol::printLinuxError()
	{
		//TODO not implemented
	}
}

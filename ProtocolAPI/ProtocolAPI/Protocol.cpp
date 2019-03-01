#include "stdafx.h"
#include "Protocol.h"
#include "Message.h"
#include <vector>

namespace ChatLib
{
	const MessageType Protocol::TrySendMessage(const Message& message,const CROSS_SOCKET& socket)
	{
		//TODO need to know sent message or not
		SendMessagee(message, socket);

		Message response = RecieveMessage(socket);

		return response.GetType();
	}

	const Message Protocol::TryRecieveMessage(const CROSS_SOCKET& socket)
	{
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(socket, &rfds);
		int maxFD = (int)socket + 1;

		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 500;
		Message message(eInvalid);

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
				message = RecieveMessage(socket);

				if(message.GetType() != eInvalid)
					SendResponse(eResponseOk, socket);
			}
		}
		return message;
	}

	 const Message Protocol::RecieveMessage(const CROSS_SOCKET& socket)
	{
		//TODO check recieved num
		char buff[MAX_PACKAGE_LENGTH];
		int recived = recv(socket, buff, MAX_PACKAGE_LENGTH, NULL);
		printf("RecieveMessage %d bytes: ", recived);
		for (int i = 0; i < recived; i++)
		{
			printf("%02X ", buff[i]);
		}
		printf("\n");
		if (recived == 0)
		{
			printf("Recieve message: connection softly closed");
			closesocket(socket);
			WSACleanup();
			//TODO error check Win and Timeout
		}
		else if (recived == -1)
		{
			printWsaError();
			closesocket(socket);
			WSACleanup();
			//TODO check Unix error
		}
		else
		{
			std::string str = "";

			MessageType type = GetMessageType(buff);

			printf("Recieved message type equal %d \n", type);

			Message message (type, buff);

			printf("Recieved message equal %s \n", message.GetText());
			
			return message;
		}
		return Message(eInvalid);
	}

	void Protocol::SendMessagee(const Message& message, const CROSS_SOCKET& socket)
	{
		char pBuffer[255];

		int sended = 0;
		int length = message.WriteBytes(pBuffer);
		printf("Sending %d bytes: ", length);
		for(int i = 0; i < length; i++)
		{
			printf("%02X ", pBuffer[i]);
		}
		printf("...\n");
		do
		{
			//TODO check with length = 1
			//TODO check & + [] + string
			sended = send(socket, pBuffer, length, NULL);
			printf("Sended %d\n", sended);
			length -= sended;
			if (sended == -1)
			{
				closesocket(socket);
				printWsaError();
				WSACleanup();
				break;
				//TODO check error WIN \ UNIX
			}
			else if(sended == 0)
			{
				break;
			}
		} while (length > 0);
	}

	//MessageType Protocol::RecieveAnswer(int socket)
	//{
	//	char recvBuffer[RESPONSE_OK_LENGTH];

	//	int recieved = 0;
	//	MessageType response = eInvalid;
	//	do
	//	{
	//		recieved = recv(socket, recvBuffer, RESPONSE_OK_LENGTH, NULL);
	//		printf("RecieveAnswer %d bytes: ", recieved);
	//		for (int i = 0; i < recieved; i++)
	//		{
	//			printf("%02X ", recvBuffer[i]);
	//		}
	//		printf("\n");
	//		if (recieved == 0)
	//		{
	//			//TODO check error or close conn
	//		}
	//		else if (recieved == -1)
	//		{
	//			//TODO checkerror
	//		}
	//		else
	//		{
	//			return response = GetMessageType(recvBuffer);
	//		}
	//		//TODO maybe while responce != oK
	//	}
	//	//while (recieved < RESPONSE_OK_LENGTH);
	//	//TODO check 
	//	while (true);
	//}

	void Protocol::SendResponse(const MessageType& messageType, const CROSS_SOCKET& socket)    //, std::string strMessage)
	{
		Message message(messageType);                                   //, strMessage);
		SendMessagee(message, socket);
	}

	const bool Protocol::IsLegalPackage(const char* buff)
	{
		return false;
	}

	//MessageType Protocol::CheckResponseStatus(char* buff)
	//{
	//	if (*(int *)buff == HEADER_START)
	//	{
	//		//TODO check
	//		if ((int)buff[4] == eResponce)
	//		{
	//			if ((int)buff[5] == eOk)
	//				return eOk;
	//			else if ((int)buff[5] == eError)
	//				return eError;
	//			else
	//			{
	//				//TODO
	//				//throw new std::exception(" Wrong Response \n");
	//				//std::cout << " Wrong Response \n";
	//			}
	//		}
	//	}
	//	else
	//	{
	//		//TODO exception
	//	}
	//}

	const MessageType Protocol::GetMessageType(const char* buff)
	{
		if (*((int*)buff) == HEADER_START)
		{
			char messageType = buff[MESSAGE_TYPE_INDEX];

			if (messageType == eNameRequest ||
				messageType == eMessageRequest ||
				messageType == eResponseOk ||
				messageType == eResponceError)
				return (MessageType)messageType;

			throw std::exception(" Not filled package type or package is trash \n");
		}
		throw std::exception(" It isn`t message \n");
	}

	//int IncomingMessageNum(SocketsVector &socketVec)
	//{
	//	fd_set rfds;
	//	FD_ZERO(&rfds);

	//	SocketsVectorIterator it;

	//	int maxNumFD = 0;
	//	for (it = socketVec.begin(); it != socketVec.end(); it++)
	//	{
	//		FD_SET(*it, &rfds);
	//		maxNumFD = max(maxNumFD, (int)*it);
	//	}
	//	maxNumFD++;

	//	timeval timeout;
	//	timeout.tv_sec = 0;
	//	timeout.tv_usec = 500;

	//	int result = select(maxNumFD, &rfds, NULL, NULL, &timeout);
	//	return result;
	//}
	//int IncomingMessageNum(int &sockfd)
	//{
	//	fd_set rfds;
	//	FD_ZERO(&rfds);
	//	FD_SET(sockfd, &rfds);
	//	int maxNumFD = sockfd + 1;

	//	timeval timeout;
	//	timeout.tv_sec = 0;
	//	timeout.tv_usec = 500;

	//	int result = select(maxNumFD, &rfds, NULL, NULL, &timeout);

	//	//TODO error check
	//	return result;
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

#include "Server.h"

bool Server::Create(string nullip, string _serverPort)
{
	if (!ResolveIPAndPort(nullptr, _serverPort.c_str())) /*nullptr for server*/
	{
		return false;
	}

	m_serverSocket = socket(m_addrInfo->ai_family, m_addrInfo->ai_socktype, m_addrInfo->ai_protocol);

	if (m_serverSocket == INVALID_SOCKET)
	{
		printf("Unable to create server socket\n");
		return false;
	}
	if (bind(m_serverSocket, m_addrInfo->ai_addr, (int)m_addrInfo->ai_addrlen) == SOCKET_ERROR)
	{
		printf("Unable to bind to server socket\n");
		return false;
	}
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("Unable to listen on server socket\n");
		return false;
	}
	u_long nonBlocking = 1;
	if (ioctlsocket(m_serverSocket, FIONBIO, &nonBlocking) == SOCKET_ERROR)
	{
		printf("Failed to set server socket to non-blocking\n");
		return false;
	}
	return true;
}

bool Server::SendData(const char* _data, int _size)
{
	if (m_clientSockets.size() == 0) { printf("No Clients"); return false; }


	for (SOCKET CS : m_clientSockets) //does all clients
	{
		int sendResult = SOCKET_ERROR;
		sendResult = send(CS, _data, _size, 0);
		if (sendResult == SOCKET_ERROR)
		{
			//printf("Data send failed");
			return false;
		}
		printf("Bytes sent %d\n", sendResult);
	}

	return true;
}

bool Server::RecvData()
{
	for (SOCKET CS : m_clientSockets) //checks all clients
	{
		int recvResult = recv(CS, recvBuf, DEFAULT_BUF_LEN, 0);
		if (recvResult > 0)
		{
			printf("Received data: %s\n", recvBuf);
			memset(recvBuf, 0, DEFAULT_BUF_LEN);
		}
	}
	return true;
}

bool Server::AcceptClient()
{
	SOCKET S = INVALID_SOCKET;
	S = accept(m_serverSocket, nullptr, nullptr);
	if (S == INVALID_SOCKET)
	{
		//printf("Unable to accept client socket\n");
		return false;
	}

	m_clientSockets.push_back(S);
	u_long block = 1; //0 = blocking
	if (ioctlsocket(m_clientSockets.back(), FIONBIO, &block) == SOCKET_ERROR) //does this need to be here?
	{
		printf("Setting non blocking failed\n");
	}
	return true;
}
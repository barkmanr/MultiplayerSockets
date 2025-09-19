#include "Client.h"

bool Client::Create(string _serverIP, string _serverPort)
{
	if (!ResolveIPAndPort(_serverIP.c_str(), _serverPort.c_str()))
	{
		return false;
	}
	if (!m_clientSockets.empty()) 
	{ 
		printf("Already have client\n");
		return false; 
	}

	m_clientSockets.push_back(socket(m_addrInfo->ai_family, m_addrInfo->ai_socktype, m_addrInfo->ai_protocol));
	if (m_clientSockets.back() == INVALID_SOCKET)
	{
		printf("Unable to create client socket\n");
		m_clientSockets.pop_back();
		return false;
	}

	//connect to server (needs server)
	if (connect(m_clientSockets.back(), m_addrInfo->ai_addr, (int)m_addrInfo->ai_addrlen) == SOCKET_ERROR)
	{
		printf("Unable to connect to server\n");
		closesocket(m_clientSockets.back());
		m_clientSockets.pop_back();
		return false;
	}

	u_long block = 1;
	if (ioctlsocket(m_clientSockets.back(), FIONBIO, &block) == SOCKET_ERROR)
	{
		printf("Setting non blocking failed\n");
	}

	printf("Winsock Client Socket successfuly connect at % u\n", m_clientSockets.back());
	return true;
}

bool Client::SendData(const char* _data, int _size)
{
	int sendResult = SOCKET_ERROR;
	if (!m_clientSockets.size() == 1) 
	{
		printf("Client not setup to send");
		return false;
	}

	sendResult = send(m_clientSockets.front(), _data, _size, 0);
	if (sendResult == SOCKET_ERROR)
	{
		printf("Data send failed");
		return false;
	}
	printf("\nBytes sent %d\n", sendResult);

	return true;
}

bool Client::RecvData()
{
	if (m_clientSockets.size() < 1) { return true; }
	int recvResult = recv(m_clientSockets.front(), recvBuf, DEFAULT_BUF_LEN, 0);
	if (recvResult > 0)
	{
		printf("Received data: %s\n", recvBuf);
		memset(recvBuf, 0, DEFAULT_BUF_LEN);
	}
	return true;
}

bool Client::AcceptClient() //does nothing, only for server 
{
	return true;
}
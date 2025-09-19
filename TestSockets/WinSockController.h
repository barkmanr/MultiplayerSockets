#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <string>
#include <vector>
#include <iostream>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUF_LEN 512

class WinSockController
{
public:
	WinSockController();
	virtual ~WinSockController();
	void Cleanup();

protected: //bad practice 
	bool ResolveIPAndPort(const char* _serverIP, const char* _serverPort);
	ADDRINFOA* m_addrInfo;
	SOCKET m_serverSocket;
	//SOCKET m_clientSocket;
	vector<SOCKET> m_clientSockets;
	char recvBuf[DEFAULT_BUF_LEN] = {}; //test for now;

	WSADATA m_data;
	BYTE m_lv; //Low-byte major
	BYTE m_hv; //High-byte minor
	BYTE m_hlv; //Max version major
	BYTE m_hhv; //Max version minor
	int m_major; //wanted
	int m_minor; //wanted

public:
	bool Initialize(int _major, int _minor);
	bool PrintErrorInfo(int _retVal);
	bool PrintMismatchInfo();
	void PrintSuccessInfo();

	virtual bool AcceptClient() =0;
	virtual bool Create(string _serverIP, string _serverPort) = 0;
	//bool CreateServer(string _serverPort);
	//bool CreateClient(string _serverIP, string _serverPort);
	virtual bool SendData(const char* _data, int size) = 0;
	virtual bool RecvData() = 0;
};


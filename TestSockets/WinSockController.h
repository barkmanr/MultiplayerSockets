#pragma once
#include <WinSock2.h>
#include <string>
#include <iostream>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

class WinSockController
{
public:
	WinSockController();
	virtual ~WinSockController();
private:
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
	void Cleanup();
};


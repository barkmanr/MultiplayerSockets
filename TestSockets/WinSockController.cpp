#include "WinSockController.h"

WinSockController::WinSockController()
{
	m_addrInfo = nullptr;
	m_serverSocket = INVALID_SOCKET;
	//m_clientSocket = INVALID_SOCKET;
	m_data = {};
	m_lv = 0;
	m_hv = 0;
	m_hlv = 0;
	m_hhv = 0;
	m_major = 0;
	m_minor = 0;
}

WinSockController::~WinSockController()
{
	Cleanup();
}

bool WinSockController::Initialize(int _major, int _minor)
{
	m_major = _major;
	m_minor = _minor;
	int test = 0;
	int retVal = WSAStartup(MAKEWORD(m_major, m_minor), &m_data);
	if (PrintErrorInfo(retVal)) { return false; }
	m_lv = LOBYTE(m_data.wVersion);
	m_hv = HIBYTE(m_data.wVersion);
	m_hlv = LOBYTE(m_data.wHighVersion);
	m_hhv = HIBYTE(m_data.wHighVersion);
	if (PrintMismatchInfo()) { return false; }
	else { PrintSuccessInfo(); }
	return true;
}

bool WinSockController::ResolveIPAndPort(const char* _serverIP, const char* _serverPort)
{
	ADDRINFO TempAdd; //Temporary address data
	ZeroMemory(&TempAdd, sizeof(TempAdd));
	TempAdd.ai_family = AF_INET; //IPV4
	TempAdd.ai_socktype = SOCK_STREAM;	//SOCK_DGRAM for UDP
	TempAdd.ai_protocol = IPPROTO_TCP; //IPPROTO_UDP for UDP
	if (_serverIP == nullptr) //For Server
	{
		TempAdd.ai_flags = AI_PASSIVE;
	}
	if (getaddrinfo(_serverIP, _serverPort, &TempAdd, &m_addrInfo) != 0) //put binary address in member address (auto does it for server)
	{
		printf("Port & IP could not be resolved\n");
		return false;
	}
	printf("Port & IP resolved\n");
	return true;
}

bool WinSockController::PrintErrorInfo(int _retVal)
{
	if (_retVal != 0)
	{
		cout << WSAGetLastError() << endl;
		switch (_retVal)
		{
		case 6:
			cout << "(6)-WSA_INVALID_HANDLE: Specified event object handle is invalid. An application attempts to use an event object, but the specified handle is not valid." << endl;
			break;
		case 8:
			cout << "(8)-WSA_NOT_ENOUGH_MEMORY: Insufficient memory available. An application used a Windows Sockets function that directly maps to a Windows function.The Windows function is indicating a lack of required memory resources." << endl;
			break;
		case 87:
			cout << "(8)-WSA_INVALID_PARAMETER: One or more parameters are invalid. An application used a Windows Sockets function which directly maps to a Windows function.The Windows function is indicating a problem with one or more parameters." << endl;
			break;
		case 10092: 
			cout << "(10092)-WSAVERNOTSUPPORTED: Winsock.dll version out of range. The current Windows Sockets implementation does not support the Windows Sockets specification version requested by the application.Check that no old Windows Sockets DLL files are being accessed." << endl;
		}
		cout << "Error: " << _retVal << endl;
		return true;
	}
	return false;
}

bool WinSockController::PrintMismatchInfo()
{
	if ((m_lv != m_major) || (m_hv != m_minor))
	{
		cout << "Incorect version initialized?" << endl;
		cout << "Asked for: " << m_major << "." << m_minor << endl;
		cout << "Actual version: " << int(m_lv) << "." << int(m_hv) << endl;
		return true;
	}
	return false;
}

void WinSockController::PrintSuccessInfo()
{
	cout << "Version: " << int(m_lv) << "." << int(m_hv) << endl;
	cout << "Highest Version: " << int(m_hlv) << "." << int(m_hhv) << endl;
	cout << "Max Sockets:  " << m_data.iMaxSockets<<  endl;
	cout << "Max UDP DG:  " << m_data.iMaxUdpDg << endl;
	if(m_data.lpVendorInfo == nullptr)
	{
		cout << "VendorInfo:  <empty>" << endl;
	}
	else 
	{
		cout << "VendorInfo:  " << string(m_data.lpVendorInfo) << endl;
	}
	cout << "Description:  " << string(m_data.szDescription) << endl;
	cout << "System Status:  " << string(m_data.szSystemStatus) << endl;
	printf("Winsock Initialized Successfully\n");
}

void WinSockController::Cleanup()
{
	WSACleanup();
	for(SOCKET s : m_clientSockets)
	{
		closesocket(s);
	}
	m_clientSockets.clear();
	if (m_serverSocket != INVALID_SOCKET)
	{
		closesocket(m_serverSocket);
		m_serverSocket = INVALID_SOCKET;
	}
	/*if (m_clientSocket != INVALID_SOCKET)
	{
		closesocket(m_clientSocket);
		m_clientSocket = INVALID_SOCKET;
	}*/
	if (m_addrInfo != nullptr)
	{
		freeaddrinfo(m_addrInfo);
		m_addrInfo = nullptr;
	}
	printf("Cleaned Winsock\n");
}

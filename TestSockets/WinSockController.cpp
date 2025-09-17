#include "WinSockController.h"

WinSockController::WinSockController()
{
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
	printf("Cleaned Winsock\n");
}

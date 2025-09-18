#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include<Windows.h>
#include <conio.h>
#include "WinSockController.h"

struct KeyState
{
	string msg = "";
	bool go = false;
	char key = ' ';
	bool status = true;
};

static WinSockController winsock = WinSockController();
const char* type;

int Initialize(char* argv[]);
void GameLoop();
void SendMsg(string msg);
KeyState HitKey(KeyState KS);
KeyState HandleKey(KeyState KS);
void HandleServer();


int main(int argc, char* argv[])
{
	//created git
	if (!(argc >= 2)) { "argc not big enough"; return 0; }
	int returnValue = Initialize(argv);
	if (returnValue >= 0)
	{
		GameLoop();
	}
	return returnValue;
}

int Initialize(char* argv[])
{
	type = argv[1];
	if (strcmp(argv[1], "server") == 0) //server 
	{
		if ((!winsock.Initialize(2, 2)) || (!winsock.CreateServer("42069")))
		{
			return -1;
		}
	}
	if (strcmp(argv[1], "client") == 0) //client 
	{
		if ((!winsock.Initialize(2, 2)) || (!winsock.CreateClient("127.0.0.1" /*local address*/,"42069")))
		{
			return -1;
		}
	}
	return 0;
}

void GameLoop()
{
	KeyState KS = KeyState{};
	while (true)
	{
		HandleServer();
		KS = HandleKey(KS);
		if (!KS.status)
		{
			break;
		}
		winsock.RecvData();
	}
		
}

void SendMsg(string msg)
{
	winsock.SendData(msg.c_str(), msg.length());
}

KeyState HitKey(KeyState KS)
{
	if (_kbhit())
	{
		KS.key = _getch();
		if (KS.key >= 32 && KS.key <= 126) //character key
		{
			KS.go = true;
		}
	}
	return KS;
}
KeyState HandleKey(KeyState KS)
{
	KS = HitKey(KS);
	if (KS.go) //character
	{
		KS.go = false;
		KS.msg += KS.key;

	}
	if (KS.key == 13) //enter
	{
		KS.key = ' ';
		SendMsg(KS.msg);
		KS.msg = "";
	}

	if (KS.key == 8) //backspace
	{
		KS.key = ' ';
		if (!KS.msg.empty()) {
			KS.msg.erase(KS.msg.length() - 1);
		}
	}
	if (KS.key == 27) //escape
	{
		KS.status = false;
	}
	return KS;
}

void HandleServer()
{
	if (strcmp(type, "server") != 0)
	{
		return;
	}
	winsock.AcceptClient();
}
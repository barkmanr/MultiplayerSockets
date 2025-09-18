#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include<Windows.h>
#include <conio.h>
#include "WinSockController.h"

static WinSockController winsock = WinSockController();

int Initialize(char* argv[]);
void GameLoop();
void SendMsg(string msg);

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
	string msg = "";
	bool Go = false;
	char key = ' ';
	while (true)
	{
		if (_kbhit())
		{
			key = _getch();
			if (key >= 32 && key <= 126) //character key
			{
				Go = true;
			}
		}
		if (Go) //character
		{
			Go = false;
			msg += key;

		}
		if (key == 13) //enter
		{
			key = ' ';
			SendMsg(msg);
			msg = "";
		}

		if (key == 8) //backspace
		{
			key = ' ';
			if (!msg.empty()) {
				msg.erase(msg.length() - 1);
			}
		}
		if (key == 27) //escape
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

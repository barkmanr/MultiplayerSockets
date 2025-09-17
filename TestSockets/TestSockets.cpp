#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include<Windows.h>
#include <conio.h>
#include "WinSockController.h"

static WinSockController winsock = WinSockController();

int Initialize(char* argv[]);
void GameLoop();

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
	if (strcmp(argv[1], "server") == 0)
	{
		if ((!winsock.Initialize(2, 2)) ||(!winsock.CreateServer("42069")))
		{
			return -1;
		}
	}
	else if (strcmp(argv[1], "client") == 0)
	{
		if ((!winsock.Initialize(2, 2)) || (!winsock.CreateClient("127.0.0.1","42069")))
		{
			return -1;
		}
		winsock.SendData("Hi Ryan B!\n", 11);
	}
	else if (strcmp(argv[1], "test") == 0)
	{
		if (!winsock.Initialize(1, 0))
		{
			return -1;
		}
	}
	return 0;
}

void GameLoop()
{
	while (true)
	{
		if (GetAsyncKeyState(27) & 0x8000) //Esc
		{
			break;
		}
		winsock.RecvData();
	}
}

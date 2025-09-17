#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include<Windows.h>
#include <conio.h>
#include "WinSockController.h"

static WinSockController winsock = WinSockController();

int Initialize(char* argv[]);
void GameLoop();
void GameLoop2();

int main(int argc, char* argv[])
{
	//c
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
		if (!winsock.Initialize(2, 3))
		{
			return -1;
		}
	}
	else if (strcmp(argv[1], "client") == 0)
	{
		if (!winsock.Initialize(2, 2))
		{
			return -1;
		}
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
	LONG i = 0;
	while (true)
	{
		i++;
		char key = _getch(); //get key ascii 
		cout << i << endl;
		if (key == 27) //Esc
		{
			break;
		}
	}
}

void GameLoop2()
{
	LONG i = 0;
	while (true)
	{
		i++;
		cout << i << endl;
		//https://stackoverflow.com/questions/41600981/how-do-i-check-if-a-key-is-pressed-on-c
		//https://devblogs.microsoft.com/oldnewthing/20041130-00/?p=37173#:~:text=When%20should%20you%20use%20GetKeyState,you%20want%20to%20use%20GetKeyState%20.
		if (GetAsyncKeyState(27) & 0x8000) //Esc
		{
			break;
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

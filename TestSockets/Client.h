#pragma once
#include "WinSockController.h"
class Client : public WinSockController
{
public:
	bool AcceptClient() override;
	bool Create(string _serverIP, string _serverPort) override;
	bool SendData(const char* _data, int size) override;
	bool RecvData() override;
};


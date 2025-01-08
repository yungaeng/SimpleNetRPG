#pragma once
#include "stdafx.h"
#include "Client.h"
#include <array>

class Hosting
{
public:
	WSADATA WSAData = {};
	SOCKET listen_socket = {};
	SOCKADDR_IN server_addr = {};
	HANDLE h_iocp = {};

	SOCKET c_socket = {};
	int addr_size = {};
	EX_OVER a_over = {};

	Hosting();
	~Hosting();

	void Init();
	void Connect();
	void Run();

private:
	void Disconnect(int id);	
	int NewID();
	void PacketHandler(int id, char* packet);
	
	int client_num();
	array<Client, MAX_CLIENT> clients;
};


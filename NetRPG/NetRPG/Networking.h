#pragma once
#pragma comment(lib, "ws2_32") // ws2_32.lib ¸µÅ©
#pragma comment(lib, "MSWSock.lib")

#include <ws2tcpip.h>
#include <mswsock.h>

#include <iostream>
#include <array>
#include <string>
#include <thread>

#include "Client.h"
#include "protocol.h"
#include "EX_OVER.h"

using namespace std;

class Networking
{
public:
	WSADATA wsadata;
	SOCKET socket = {};
	SOCKADDR_IN socket_addr = {};
	HANDLE h_iocp = {};
	EX_OVER a_over = {};
	
	SOCKET c_socket = {};
	int addr_size;

public:
	bool is_connect = true;
	string name;

private:
	int id;
	
public:
	void Host(int port);
	void Join(const char* addr, int port);

private:
	void Init(int port);
	void RunHost();
	void RunJoin();

	void RecvPacketHandler(int id, char* packet, array<Client, MAX_CLIENT> client);
	void SendPacketHandler(char packet_id);
};


#pragma once
#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ
#pragma comment(lib, "MSWSock.lib")

#include <ws2tcpip.h>
#include <mswsock.h>

#include <iostream>
#include "protocol.h"


enum OP
{
    ACCEPT, DISCONNECT,
    SEND, RECV
};

//  Ȯ��� overlapped
class EX_OVER
{
public:
    WSAOVERLAPPED over = {};
    WSABUF wsabuf = {};
    char buf[BUFFER_SIZE] = {};
    OP op = {};
    EX_OVER();
    EX_OVER(unsigned char* packet);
};




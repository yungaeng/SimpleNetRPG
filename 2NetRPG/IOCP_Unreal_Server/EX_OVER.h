#pragma once
#include "stdafx.h"

enum OP
{
    ACCEPT, DISCONNECT,
    SEND, RECV
};

//  »Æ¿Âµ» overlapped
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


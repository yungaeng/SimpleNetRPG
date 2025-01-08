#include "EX_OVER.h"

EX_OVER::EX_OVER()
{
    wsabuf.len = BUFFER_SIZE;
    wsabuf.buf = buf;
    op = RECV;
    ZeroMemory(&over, sizeof(over));
}

EX_OVER::EX_OVER(unsigned char* packet)
{
    wsabuf.len = packet[0];
    wsabuf.buf = buf;
    ZeroMemory(&over, sizeof(over));
    op = SEND;
    memcpy(buf, packet, packet[0]);
}

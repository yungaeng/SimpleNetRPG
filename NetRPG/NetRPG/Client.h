#pragma once
#pragma comment(lib, "ws2_32") // ws2_32.lib ¸µÅ©
#pragma comment(lib, "MSWSock.lib")

#include <ws2tcpip.h>
#include <mswsock.h>

#include <iostream>
#include "protocol.h"
#include "EX_OVER.h"

class Client
{
public:
    SOCKET m_socket = {};
    int m_id = {};
    bool m_login = false;
    char m_name[MAX_NAME_LEN] = {};
    char m_chat[MAX_CHAT_LEN] = {};
    float m_loc_x, m_loc_y, m_loc_z = 0;
    float m_rot_x, m_rot_y, m_rot_z = 0;
    EX_OVER m_exover = {};

    int m_prev_size = 0;
public:
    void do_send(void* packet) const;
    void do_recv();

    void sc_login_ok();
    void sc_move();
};




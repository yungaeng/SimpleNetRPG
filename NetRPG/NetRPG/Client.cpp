#include "Client.h"
#include "protocol.h"

void Client::do_send(void* packet) const
{
    EX_OVER* sdata = new EX_OVER{ reinterpret_cast<unsigned char*>(packet) };
    WSASend(m_socket, &sdata->wsabuf, 1, 0, 0, &sdata->over, 0);
}

void Client::do_recv()
{
    DWORD recv_flag = 0;
    memset(&m_exover.over, 0, sizeof(m_exover.over));
    m_exover.wsabuf.len = BUFFER_SIZE - m_prev_size;
    m_exover.wsabuf.buf = m_exover.buf + m_prev_size;
    WSARecv(m_socket, &m_exover.wsabuf, 1, 0, &recv_flag,
        &m_exover.over, 0);
}

void Client::sc_login_ok()
{
    SC_LOGIN_OK_PACKET p;
    p.size = sizeof(SC_LOGIN_OK_PACKET);
    p.type = SC_LOGIN_OK;

    p.id = m_id;

    p.loc_x = m_loc_x;
    p.loc_y = m_loc_y;
    p.loc_z = m_loc_z;

    p.rot_x = m_rot_x;
    p.rot_y = m_rot_y;
    p.rot_z = m_rot_z;
    do_send(&p);
}

void Client::sc_move()
{
    SC_MOVE_PACKET p;
    p.size = sizeof(SC_MOVE_PACKET);
    p.type = SC_MOVING;

    p.id = m_id;
    do_send(&p);
}


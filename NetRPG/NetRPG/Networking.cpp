#include "Networking.h"

//--------------------------------------------------------------------------//

int NewID(array<Client, MAX_CLIENT> clients)
{
    for (int i = 1; i <= MAX_CLIENT; ++i)
        if (clients[i].m_login == false)
            return i;
    return -1;
}

void Disconnect(int id, array<Client, MAX_CLIENT> clients)
{
    for (auto& cl : clients) {
        if (cl.m_login == false) continue;
        if (cl.m_id == id) continue;
        SC_LEAVE_PACKET p;
        p.id = id;
        p.size = sizeof(p);
        p.type = DISCONNECT;
        cl.do_send(&p);
    }
    if (id != NULL)
        closesocket(clients[id].m_socket);
    clients[id].m_login = false;
}

void recv_thread(SOCKET socket, bool is_connect, int id)
{
    while (is_connect)
    {
        char buf[BUFFER_SIZE];
        int ret = recv(socket, buf, sizeof(buf), 0);
        if (ret > 0)
        {
            switch (buf[1])
            {
            case SC_LOGIN_OK:
            {
                SC_LOGIN_OK_PACKET* p = reinterpret_cast<SC_LOGIN_OK_PACKET*>(buf);
                cout << "사용자 아이디 : " << p->id << " 지금 서버에 접속했습니다" << endl;
                id = p->id;
                break;
            }
            case SC_ENTER:
            {
                SC_ENTER_PACKET* p = reinterpret_cast<SC_ENTER_PACKET*>(buf);
                cout << "아이디 : " << p->id << " 이름 : " << p->name << "(이)가 접속 중 입니다." << endl;
                cout << "loc : " << p->loc_x << ", " << p->loc_y << ", " << p->loc_z << endl;
                cout << "loc : " << p->rot_x << ", " << p->rot_y << ", " << p->rot_z << endl;
                break;
            }
            case SC_MOVE:
            {
                SC_MOVE_PACKET* p = reinterpret_cast<SC_MOVE_PACKET*>(buf);
                cout << "아이디 : " << p->id << "(이)가 이동합니다." << endl;
                cout << "key : " << p->dir << endl;

                break;
            }
            case SC_LEAVE:
            {
                SC_LEAVE_PACKET* p = reinterpret_cast<SC_LEAVE_PACKET*>(buf);
                cout << "아이디 : " << p->id << "(이)가 접속 종료했습니다" << endl;
                break;
            }
            default:
                break;
            }
        }
        else
        {
            cout << "연결에 실패했습니다." << endl;
            is_connect = false;
            closesocket(socket);
        }
    }

}

int client_num(array<Client, MAX_CLIENT> clients)
{
    int num = 0;

    for (auto& cl : clients)
        if (cl.m_login)
            num++;
    return num;
}

//--------------------------------------------------------------------------//

void Networking::Host(int port)
{
    Init(port);

    socket_addr.sin_addr.S_un.S_addr = INADDR_ANY;
    if (bind(socket, reinterpret_cast<sockaddr*>(&socket_addr), sizeof(socket_addr)))
    {
        cout << "bind 실패" << endl;
        return;
    }
    if (listen(socket, SOMAXCONN))
    {
        cout << "listen 실패" << endl;
        return;
    }

    // 로그 출력
    cout << "Server is Ready!!" << endl;
    cout << "SERVER PORT : " << socket_addr.sin_port << endl;
    cout << "MAX USER : " << MAX_CLIENT << endl;
    cout << endl;

    SOCKADDR_IN cl_addr{};
    addr_size = sizeof(cl_addr);
    int client_id = 0;

    h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
    CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket), h_iocp, 9999, 0);

    // 새 클라이언트 소켓
    c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    a_over.op = ACCEPT;
    AcceptEx(socket, c_socket, a_over.buf, 0, addr_size + 16, addr_size + 16, 0, &a_over.over);

    RunHost();
}

void Networking::Join(const char* addr, int port)
{
    Init(port);

    inet_pton(AF_INET, addr, &socket_addr.sin_addr);

    if (connect(socket, reinterpret_cast<sockaddr*>(&socket_addr), sizeof(socket_addr)) == SOCKET_ERROR)
    {
        cout << "연결이 끊겼습니다." << endl;
        is_connect = false;
        closesocket(socket);
        WSACleanup();
        return;
    }

    RunJoin();
}

void Networking::Init(int port)
{
    if (WSAStartup(MAKEWORD(2, 2), &wsadata))
    {
        cout << "wsa 초기화 실패" << endl;
        return;
    }
    socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

    memset(&socket_addr, 0, sizeof(socket_addr));
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(port);
}

void Networking::RunHost()
{
    array<Client, MAX_CLIENT> clients;

    while (true) {
        DWORD num_bytes;
        ULONG_PTR key;
        WSAOVERLAPPED* over = nullptr;
        BOOL ret = GetQueuedCompletionStatus(h_iocp, &num_bytes, &key, &over, INFINITE);
        EX_OVER* ex_over = reinterpret_cast<EX_OVER*>(over);

        if (FALSE == ret) {
            if (ex_over->op == ACCEPT) {
                cout << "Accept error";
                exit(-1);
            }
            else {
                cout << "GQCS error on client[" << key << "]\n";
                Disconnect(static_cast<int>(key), clients);
                if (ex_over->op == SEND) delete ex_over;
                continue;
            }
        }

        switch (ex_over->op) {
        case ACCEPT:
        {
            int client_id = NewID(clients);
            if (client_id != -1) {
                clients[client_id].m_login = true;
                clients[client_id].m_id = client_id;
                clients[client_id].m_name[0] = 0;
                clients[client_id].m_prev_size = 0;
                clients[client_id].m_socket = c_socket;
                CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket),
                    h_iocp, client_id, 0);

                clients[client_id].do_recv();
            }
            else {
                cout << "Maximum number of users reached" << endl;
                closesocket(c_socket);
            }
            c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
            ZeroMemory(&a_over.over, sizeof(a_over.over));
            AcceptEx(socket, c_socket, a_over.buf, 0, addr_size + 16, addr_size + 16, 0, &a_over.over);
            break;
        }
        case RECV:
        {
            int remain_data = num_bytes + clients[key].m_prev_size;
            char* p = ex_over->buf;

            while (remain_data > 0) {
                int packet_size = p[0];
                if (packet_size <= remain_data) {
                    RecvPacketHandler(static_cast<int>(key), p, clients);
                    p = p + packet_size;
                    remain_data = remain_data - packet_size;
                }
                else break;
            }

            clients[key].m_prev_size = remain_data;

            if (remain_data > 0)
                memcpy(ex_over->buf, p, remain_data);

            clients[key].do_recv();
            break;
        }
        case SEND:
        {
            delete ex_over;
            break;
        }
        case DISCONNECT:
        {
            cout << "Client[" << key << "] is DisConnect!!" << endl;
            Disconnect(static_cast<int>(key), clients);
            if (ex_over->op == SEND) delete ex_over;
            break;
        }
        closesocket(socket);
        WSACleanup();
        }
    }
}

void Networking::RunJoin()
{
    thread recvThread(recv_thread, socket, is_connect, id);

    SendPacketHandler(CS_LOGIN);

    // 송신 루프
    while (is_connect)
    {
        string message;
        getline(cin, message);

        if (message == "/quit")
        {
            is_connect = false;
            break;
        }
    }

    // 수신작업 종료대기
    recvThread.join();
}

void Networking::RecvPacketHandler(int id, char* packet, array<Client, MAX_CLIENT> clients)
{
    switch (packet[1])
    {
    case CS_LOGIN:
    {
        CS_LOGIN_PACKET* p = reinterpret_cast<CS_LOGIN_PACKET*>(packet);
        strcpy_s(clients[id].m_name, p->name);
        clients[id].sc_login_ok();
        // 로그 띄우기 

        cout << "Client " << p->name << " is LOGIN!!" << endl;
        cout << "ID : " << id << endl;
        cout << "CLIENT NUM : " << client_num(clients) << endl;
        cout << endl;

        // 새 클라이언트가 로그인 된 클라이언트에게 로그인 정보 전송
        for (auto& cl : clients) {
            if (false == cl.m_login) continue;
            if (cl.m_id == id) continue;
            SC_ENTER_PACKET enter_packet{};
            enter_packet.size = sizeof(enter_packet);
            enter_packet.type = SC_ENTER;
            enter_packet.id = id;
            strcpy_s(enter_packet.name, p->name);
            cl.do_send(&enter_packet);
        }

        // 로그인되어있는 모든 클라이언트의 정보를 로그인한 클라이언트에게 전달
        for (auto& cl : clients) {
            if (false == cl.m_login) continue;
            if (cl.m_id == id) continue;
            SC_ENTER_PACKET enter_packet;
            enter_packet.size = sizeof(enter_packet);
            enter_packet.type = SC_ENTER;
            enter_packet.id = cl.m_id;

            enter_packet.loc_x = cl.m_loc_x;
            enter_packet.loc_y = cl.m_loc_y;
            enter_packet.loc_z = cl.m_loc_z;

            enter_packet.rot_x = cl.m_rot_x;
            enter_packet.rot_y = cl.m_rot_y;
            enter_packet.rot_z = cl.m_rot_z;
            strcpy_s(enter_packet.name, cl.m_name);
            clients[id].do_send(&enter_packet);
        }
        break;
    }
    case CS_MOVING:
    {
        CS_MOVING_PACKET* p = reinterpret_cast<CS_MOVING_PACKET*>(packet);
        clients[id].sc_move();

        // 로그 띄우기 
        cout << "Client ID : " << id << " is MOVING!!" << endl;
        cout << "Input : " << p->key << endl;

        // 모든 클라이이언트 들에게 패킷 전송하기
        for (auto& cl : clients) {
            if (false == cl.m_login) continue;
            if (cl.m_id == id) continue;
            SC_MOVE_PACKET move_packet;
            move_packet.size = sizeof(move_packet);
            move_packet.type = SC_MOVING;
            move_packet.id = id;
            move_packet.dir = p->key;
            cl.do_send(&move_packet);
        }
        break;
    }
    default:
        break;
    }
}

void Networking::SendPacketHandler(char packet_id)
{
    switch (packet_id)
    {
    case CS_LOGIN:
        CS_LOGIN_PACKET login_packet;
        login_packet.size = sizeof(login_packet);
        login_packet.type = CS_LOGIN;
        strcpy_s(login_packet.name, name.c_str());
        send(socket, reinterpret_cast<char*>(&login_packet), sizeof(login_packet), 0);
        break;
    case CS_MOVING:
        CS_MOVING_PACKET p;
        p.size = sizeof(p);
        p.type = CS_MOVING;
        p.key = (_fgetchar());
        send(socket, reinterpret_cast<char*>(&p), sizeof(p), 0);
        break;
    default:
        break;
    }
}


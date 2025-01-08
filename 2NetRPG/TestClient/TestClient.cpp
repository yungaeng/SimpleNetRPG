#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")

#include <iostream>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <thread>
#include <string>
#include "..\..\IOCP_Unreal_Server\IOCP_Unreal_Server\protocol.h"
using namespace std;

SOCKET client_socket;
bool is_connected = true;
int local_id = {};

static void recv_thread()
{
    while (is_connected)
    {
        char buf[BUFFER_SIZE];
        int ret = recv(client_socket, buf, sizeof(buf), 0);
        if (ret > 0)
        {
            switch (buf[1])
            {
            case SC_LOGIN_OK:
            {
                SC_LOGIN_OK_PACKET* p = reinterpret_cast<SC_LOGIN_OK_PACKET*>(buf);
                cout << "사용자 아이디 : " << p->id << " 지금 서버에 접속했습니다" << endl;
                local_id = p->id;
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
            is_connected = false;
            closesocket(client_socket);
        }
    }
    
}

static void send_login(const string& name)
{
    CS_LOGIN_PACKET login_packet;
    login_packet.size = sizeof(login_packet);
    login_packet.type = CS_LOGIN;
    strcpy_s(login_packet.name, name.c_str());
    send(client_socket, reinterpret_cast<char*>(&login_packet), sizeof(login_packet), 0);
}

static void send_moving()
{
    CS_MOVING_PACKET p;
    p.size = sizeof(p);
    p.type = CS_MOVING;
    p.key = (_fgetchar());
    send(client_socket, reinterpret_cast<char*>(&p), sizeof(p), 0);
}

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
        return -1;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(client_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR)
    {
        cout << "연결이 끊겼습니다." << endl;
        is_connected = false;
        closesocket(client_socket);
        WSACleanup();
        return -1;
    }

    // 이후 서버와 연결 됨
    cout << "서버를 찾았습니다!!" << endl;
    
    // 수신 루프
    thread recvThread(recv_thread);

    cout << "이름을 입력하세요 : ";
    string name;
    cin >> name;

    // 로그인 패킷 전송
    send_login(name);

    cout << "서버에 로그인 했습니다!!" << endl;

    

    // 송신 루프
    while (is_connected)
    {
        string message;
        getline(cin, message);

        if (message == "/quit")
        {
            is_connected = false;
            break;
        }
        if (message == "move")
        {
            send_moving();
        }
    }

    // 수신작업 종료대기
    recvThread.join();

    closesocket(client_socket);
    WSACleanup();
    return 0;
}

#include "Joinning.h"

Joinning::Joinning()
{
}

Joinning::~Joinning()
{
    closesocket(client_socket);
    WSACleanup();
}

void Joinning::Init()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
        return;

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
        return;
    }
}

void Joinning::Run()
{
    // 이후 서버와 연결 됨
    cout << "서버를 찾았습니다!!" << endl;
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
}

void Joinning::send_login(const string& name)
{
    CS_LOGIN_PACKET login_packet;
    login_packet.size = sizeof(login_packet);
    login_packet.type = CS_LOGIN;
    strcpy_s(login_packet.name, name.c_str());
    send(client_socket, reinterpret_cast<char*>(&login_packet), sizeof(login_packet), 0);
}

void Joinning::send_moving()
{
    CS_MOVING_PACKET p;
    p.size = sizeof(p);
    p.type = CS_MOVING;
    p.key = (_fgetchar());
    send(client_socket, reinterpret_cast<char*>(&p), sizeof(p), 0);
}



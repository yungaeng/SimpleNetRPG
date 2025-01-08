#include <iostream>
#include <thread>

#include "Hosting.h"
#include "Joinning.h"

using namespace std;

Hosting h;
Joinning j;

void recv_thread()
{
    while (j.is_connected)
    {
        char buf[BUFFER_SIZE];
        int ret = recv(j.client_socket, buf, sizeof(buf), 0);
        if (ret > 0)
        {
            switch (buf[1])
            {
            case SC_LOGIN_OK:
            {
                SC_LOGIN_OK_PACKET* p = reinterpret_cast<SC_LOGIN_OK_PACKET*>(buf);
                cout << "사용자 아이디 : " << p->id << " 지금 서버에 접속했습니다" << endl;
                j.local_id = p->id;
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
            j.is_connected = false;
            closesocket(j.client_socket);
        }
    }

}
int main()
{
	cout << "Select Game Mod." << endl;
	cout << "1. Host" << endl;
	cout << "2. Join" << endl;

	int s;
	cin >> s;

    // 수신 루프
    thread recvThread(recv_thread);
	
    switch (s)
	{
		system("cls");
	case 1:
		h.Init();
		h.Connect();
		h.Run();
		break;
	case 2:
		j.Init();
		j.Run();
		break;
	default:
		break;
	}

    // 수신작업 종료대기
    recvThread.join();
}
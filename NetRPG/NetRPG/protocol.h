#pragma once

#define MAX_CLIENT 100
#define MAX_NAME_LEN 20
#define MAX_CHAT_LEN 20
#define BUFFER_SIZE 256

#define PORT 9000

// 클라가 서버에게 (서버에서 처리)
constexpr char CS_LOGIN = 0;
constexpr char CS_MOVING = 1;

// 서버가 클라에게 (클라에서 처리)
constexpr char SC_LOGIN_OK = 2;
constexpr char SC_ENTER = 3;
constexpr char SC_MOVING = 4;
constexpr char SC_LEAVE = 5;

#pragma pack(push, 1)

struct CS_LOGIN_PACKET
{
    unsigned char size;
    char type;
    char name[MAX_NAME_LEN];
};

struct CS_MOVING_PACKET
{
    unsigned char size;
    char type;
    char key;
};

struct SC_LOGIN_OK_PACKET
{
    unsigned char size;
    char type;
    short id;
    float loc_x, loc_y, loc_z;
    float rot_x, rot_y, rot_z;
};

struct SC_ENTER_PACKET
{
    unsigned char size;
    char type;
    short id;
    char name[MAX_NAME_LEN];
    float loc_x, loc_y, loc_z;
    float rot_x, rot_y, rot_z;
};

struct SC_MOVE_PACKET
{
    unsigned char size;
    char type;
    short id;
    char dir;
};

struct SC_LEAVE_PACKET
{
    unsigned char size;
    char type;
    short id;
};

#pragma pack(pop)

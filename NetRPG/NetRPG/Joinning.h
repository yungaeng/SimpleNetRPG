#pragma once
#include <string>
#include "stdafx.h"

class Joinning
{
public:
	Joinning();
	~Joinning();
	
	void Init();
	void Run();

    SOCKET client_socket;
    bool is_connected = true;
    int local_id = {};

private:
    void send_login(const string& name);
    void send_moving();
};


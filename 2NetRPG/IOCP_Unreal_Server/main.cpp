#include "Server.h"

int main()
{
	Server MainServer;

	MainServer.Connect();
	MainServer.Run();

	return 0;
}
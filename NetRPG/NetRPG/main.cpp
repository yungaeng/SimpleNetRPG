#include <iostream>
#include "networking.h"

using namespace std;

Networking net;

int main()
{
    system("cls");
    cout << "Insert Name : ";
    cin >> net.name;

    cout << "Select Game Mod." << endl;
	cout << "1. Host" << endl;
	cout << "2. Join" << endl;

	int s;
	cin >> s;

    switch (s)
    {
    case 1:
        net.Host(9000);
        break;
    case 2:
        net.Join("127.0.0.1", 9000);
        break;
    default:
        break;
    }
}
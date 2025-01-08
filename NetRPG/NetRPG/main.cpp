#include <iostream>

#include "Game.h"

using namespace std;

int main()
{
	cout << "Select Game Mod." << endl;
	cout << "1. Host" << endl;
	cout << "2. Join" << endl;

	int s;
	cin >> s;

	Game game;
	game.Init(s);
	game.Run();
}
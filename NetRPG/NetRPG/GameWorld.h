#pragma once
#include <vector>

#include "Character.h"
#include "Monster.h"

using namespace std;

class GameWorld
{
public:
	vector<Character> Characters;
	vector<Monster> Monsters;
public:
	void CreateGame();
	void RunGame();

	void update();
};


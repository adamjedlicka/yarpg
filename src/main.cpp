#include <fstream>
#include <iostream>
#include <sstream>

#include "engine.h"
#include "map.h"
#include "player.h"

using namespace std;

int main() {
	Engine e;

	Player *p = new Player("Winty");
	Map *l1 = new Map("level_1");
	l1->LoadFromFile("NULL");
	l1->SpawnPlayer(p);

	e.LoadLevel(l1);

	e.Start();

	return 0;
}
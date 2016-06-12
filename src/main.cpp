#include <fstream>
#include <iostream>
#include <sstream>

#include "engine.h"
#include "map.h"
#include "player.h"

using namespace std;

int main() {
	Player *player = new Player("Winty");
	Map *level = new Map("level_1");
	bool ok = level->LoadFromFile("level_1.dat");
	if (!ok) {
		cout << "Error during map loading..." << endl;
		return 1;
	}
	level->SpawnPlayer(player);

	Engine e;
	e.LoadLevel(level);
	e.Start();

	

	delete level;
	delete player;

	return 0;
}
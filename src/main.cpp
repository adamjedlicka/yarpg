#include <fstream>
#include <iostream>
#include <sstream>

#include "engine.h"
#include "map.h"
#include "player.h"

using namespace std;

int main() {
	Player *p = new Player("Winty");
	Map *l1 = new Map("level_1");
	bool ok = l1->LoadFromFile("level_1.dat");
	if (!ok) {
		cout << "Error during map loading..." << endl;
		return 1;
	}
	l1->SpawnPlayer(p);

	Engine e;
	e.LoadLevel(l1);
	e.Start();

	return 0;
}
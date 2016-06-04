#include "engine.h"
#include "map.h"

int main() {
	Engine e;
	Map *l1 = new Map("level_1");
	e.LoadLevel(l1);

	e.Start();

	return 0;
}
#ifndef __Engine_H
#define __Engine_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <unistd.h>

const uint8_t FPS = 30; // Maximum FPS. Used to calculate sleep time between frames

class Engine {
  private:
	WINDOW *window;
	std::ofstream log;

	bool active;
	int tmpX, tmpY;

  public:
	Engine();
	~Engine();

	void Start();
	void Stop();

  private:
	void loop();
	void render();
};

#endif

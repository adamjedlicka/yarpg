#include "engine.h"

Engine::Engine() {
	this->window = initscr();
	nodelay(this->window, true);
	keypad(this->window, true);
	noecho();
	cbreak();
	timeout(1);
	wtimeout(this->window, 1);
	curs_set(false);

	this->log.open("log.txt");

	this->active = false;
	this->tmpX = 0;
	this->tmpY = 0;
}

Engine::~Engine() {
	endwin();
	this->log.close();
}

void Engine::Start() {
	this->active = true;
	this->loop();
}

void Engine::Stop() { this->active = false; }

void Engine::loop() {
	clock_t drawTime = clock();

	while (this->active) {
		this->render();

		// Calculate sleep time for the rest of the frame, to save CPU load
		clock_t now = clock();
		double tm = double(now - drawTime) / CLOCKS_PER_SEC;
		usleep((1.0 / FPS - tm) * 1000000);
	}
}

void Engine::render() {
	mvprintw(this->tmpY++, this->tmpX++, "Hello, World");
	refresh();
}

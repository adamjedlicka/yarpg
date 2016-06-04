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
	for (uint16_t i = 0; i < MAX_KEYS; ++i)
		this->keys[i] = false;

	this->maxHeight = getmaxy(this->window);
	this->maxWidth = getmaxx(this->window);
	for (uint16_t i = 0; i < MAX_BUFFERS; ++i)
		this->buffers[i] = new Buffer(this->maxWidth, this->maxHeight);

	this->active = false;
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

Engine::Buffer *Engine::GetCurBuffer() { return this->buffers[0]; }

Engine::Buffer::Buffer(){};
Engine::Buffer::Buffer(uint16_t width, uint16_t height) {
	this->width = width;
	this->height = height;
	this->canvas = new char *[height];
	for (uint16_t i = 0; i < height; ++i)
		this->canvas[i] = new char[width];
}

Engine::Buffer::~Buffer() {
	for (uint16_t i = 0; i < this->height; ++i)
		delete[] this->canvas[i];
	delete[] this->canvas;
}

void Engine::Buffer::DrawChar(uint16_t height, uint16_t width, char c) {
	switch (c) {
	case '\0':
		break;
	default:
		this->canvas[height][width] = c;
	}
}

void Engine::Buffer::ClearCanvas() {
	for (uint16_t i = 0; i < this->height; ++i) {
		for (uint16_t j = 0; j < this->width; j++) {
			this->canvas[i][j] = ' ';
		}
	}
}

void Engine::loop() {
	clock_t drawTime = clock();

	while (this->active) {
		// Store all key presses in keyboard for future querries
		int c = 0;
		for (uint16_t i = 0; i < MAX_KEYS; ++i)
			this->keys[i] = false;
		while (c = getch(), c != ERR)
			this->keys[c] = true;

		// Game logic and rendering
		this->tick();
		this->render();

		// Calculate sleep time for the rest of the frame, to save CPU load
		clock_t now = clock();
		double tm = double(now - drawTime) / CLOCKS_PER_SEC;
		usleep((1.0 / MAX_FPS - tm) * 1000000);
	}
}

void Engine::tick() {}

void Engine::render() {
	Buffer *buf = this->GetCurBuffer();
	buf->ClearCanvas();

	

	for (uint16_t i = 0; i < buf->GetHeight(); ++i) {
		mvprintw(i, 0, buf->GetLine(i));
	}

	refresh();
}

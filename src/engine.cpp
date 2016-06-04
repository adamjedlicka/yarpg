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

	this->maxHeight = getmaxy(this->window);
	this->maxWidth = getmaxx(this->window);
	for (int i = 0; i < MAX_BUFFERS; ++i)
		this->buffers[i] = new Buffer(this->maxWidth, this->maxHeight);

	this->active = false;
	this->debug = false;
	this->tickCount = 0;
}

Engine::~Engine() { endwin(); }

void Engine::Start() {
	this->active = true;
	this->loop();
}

void Engine::Stop() { this->active = false; }

bool Engine::LoadLevel(Level *level) {
	this->level = level;

	return true;
}

Buffer *Engine::GetCurBuffer() { return this->buffers[0]; }

Buffer::Buffer() {}

Buffer::Buffer(int width, int height) {
	this->width = width;
	this->height = height;
	this->canvas = new char *[height];
	for (int i = 0; i < height; ++i)
		this->canvas[i] = new char[width];
}

Buffer::~Buffer() {
	for (int i = 0; i < this->height; ++i)
		delete[] this->canvas[i];
	delete[] this->canvas;
}

void Buffer::DrawChar(int height, int width, char c) {
	if (height < 0 || width < 0)
		return;
	if (height >= this->height || width >= this->width)
		return;

	switch (c) {
	case '\0':
	case '\t':
		break;
	default:
		this->canvas[height][width] = c;
	}
}

void Buffer::DrawString(int height, int width, const std::string &str) {
	int w = 0;
	int h = 0;
	for (unsigned int i = 0; i < str.length(); ++i) {
		if (str[i] == '\n') {
			++h;
			w = 0;
			continue;
		}
		this->DrawChar(height + h, width + w++, str[i]);
	}
}

void Buffer::ClearCanvas() {
	for (int i = 0; i < this->height; ++i) {
		for (int j = 0; j < this->width; j++) {
			this->canvas[i][j] = ' ';
		}
	}
}

void Engine::loop() {
	clock_t drawTime = clock();

	while (this->active) {
		// Store all key presses in keyboard for future querries
		int c = 0;
		for (int i = 0; i < MAX_KEYS; ++i)
			this->keys[i] = false;
		while (c = getch(), c != ERR)
			this->keys[c] = true;

		// Game logic and rendering
		this->tick();
		this->tickCount++;
		this->render();

		// Calculate sleep time for the rest of the frame, to save CPU load
		clock_t now = clock();
		double tm = double(now - drawTime) / CLOCKS_PER_SEC;
		usleep((1.0 / MAX_FPS - tm) * 1000000);
		drawTime = clock();
	}
}

void Engine::tick() {
	if (this->GetKey(KEY_F(2)))
		this->debug = !this->debug;

	if (this->level)
		this->level->Tick(this);
}

void Engine::render() {
	Buffer *buf = this->GetCurBuffer();
	buf->ClearCanvas();

	if (this->level)
		this->level->Render(buf);

	if (this->debug) {
		std::stringstream ss;
		ss << "tickCount: " << this->tickCount;
		buf->DrawString(0, 0, ss.str());
	}

	for (int i = 0; i < buf->GetHeight(); ++i) {
		mvprintw(i, 0, buf->GetLine(i));
	}

	refresh();
}

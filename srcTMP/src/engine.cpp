#include "engine.h"

Engine::Engine() {
	window = initscr();
	nodelay(window, true);
	keypad(window, true);
	noecho();
	cbreak();
	timeout(1);
	wtimeout(window, 1);
	curs_set(false);

	_COLOR = has_colors();
	if (_COLOR) {
		start_color();
		init_pair(1, COLOR_WHITE, COLOR_BLACK); // fixes a bug with blinking text
	}

	maxHeight = getmaxy(window);
	maxWidth = getmaxx(window);
	for (int i = 0; i < MAX_BUFFERS; ++i)
		buffers[i] = new Buffer(maxWidth, maxHeight);

	active = false;
	debug = false;
	tickCount = 0;
}

Engine::~Engine() { endwin(); }

void Engine::Start() {
	active = true;
	loop();
}

void Engine::Stop() { active = false; }

bool Engine::LoadLevel(Level *lvl) {
	level = lvl;
	return true;
}

Buffer *Engine::GetCurBuffer() { return buffers[0]; }

Buffer::Buffer() {}

Buffer::Buffer(int w, int h) {
	width = w;
	height = h;
	canvas = new Cell *[height];
	for (int i = 0; i < height; ++i)
		canvas[i] = new Cell[width];
}

Buffer::~Buffer() {
	for (int i = 0; i < height; ++i)
		delete[] canvas[i];
	delete[] canvas;
}

void Buffer::DrawChar(int x, int y, char c) { DrawChar(x, y, c, COLOR_WHITE, COLOR_BLACK); }
void Buffer::DrawChar(int x, int y, char c, short fg) { DrawChar(x, y, c, fg, COLOR_BLACK); }
void Buffer::DrawChar(int x, int y, char c, short fg, short bg) {
	if (x < 0 || y < 0)
		return;
	if (x >= width || y >= height)
		return;

	switch (c) {
	case '\0':
	case '\t':
		break;
	default:
		canvas[y][x].ch = c;
		canvas[y][x].fg = fg;
		canvas[y][x].bg = bg;
		break;
	}
}

void Buffer::DrawString(int width, int height, const std::string &str) {
	int w = 0;
	int h = 0;
	for (unsigned int i = 0; i < str.length(); ++i) {
		if (str[i] == '\n') {
			++h;
			w = 0;
			continue;
		}
		DrawChar(width + w++, height + h, str[i]);
	}
}

void Buffer::ClearCanvas() {
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; j++) {
			canvas[i][j].ch = ' ';
			canvas[i][j].fg = COLOR_WHITE;
			canvas[i][j].bg = COLOR_BLACK;
		}
	}
}

void Engine::loop() {
	clock_t drawTime = clock();

	while (active) {
		// Store all key presses in keyboard for future querries
		int c = 0;
		for (int i = 0; i < MAX_KEYS; ++i)
			keys[i] = false;
		while (c = getch(), c != ERR)
			keys[c] = true;

		// Game logic and rendering
		tick();
		tickCount++;
		render();

		// Calculate sleep time for the rest of the frame, to save CPU load
		clock_t now = clock();
		double tm = double(now - drawTime) / CLOCKS_PER_SEC;
		usleep((1.0 / MAX_FPS - tm) * 1000000);
		drawTime = clock();
	}
}

void Engine::tick() {
	if (GetKey(KEY_F(2)))
		debug = !debug;

	log << "tickCount: " << tickCount << std::endl; // DEBUG

	if (level)
		level->Tick(this);
}

void Engine::render() {
	Buffer *buf = GetCurBuffer();
	buf->ClearCanvas();

	attron(COLOR_PAIR(1));

	if (level)
		level->Render(buf);

	if (debug)
		buf->DrawString(0, 0, log.str());
	log.str("");

	int usedColorPairs = 1; // 1 because we already initialized
	for (int i = 0; i < buf->GetHeight(); ++i) {
		const Cell *line = buf->GetLine(i);
		for (int j = 0; j < buf->GetWidth(); ++j) {

			int index = 1;
			if (_COLOR) {
				short fg, bg;
				for (int i = 1; i <= usedColorPairs; ++i) {
					pair_content(i, &fg, &bg);
					if (fg == line[j].fg && bg == line[j].bg) {
						index = i;
						break;
					}
					if (i == usedColorPairs) {
						index = ++usedColorPairs;
						init_pair(index, line[j].fg, line[j].bg);
						break;
					}
				}
			}

			attron(COLOR_PAIR(index));
			mvaddch(i, j, line[j].ch);
			attroff(COLOR_PAIR(index));
		}
	}

	attron(COLOR_PAIR(1));

	if (_COLOR) {
		log << "max color_pairs: " << COLOR_PAIRS - 1 << std::endl;
		log << "used color_pairs: " << usedColorPairs << std::endl;
	}

	refresh();
}

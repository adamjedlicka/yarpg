#include "engine.h"

Engine::Engine() {
	window = initscr();
	nodelay(window, true);
	keypad(window, true);
	noecho();
	// cbreak();
	// timeout(1);
	// wtimeout(window, 1);
	curs_set(0);
	// raw();
	// clear();

	_COLOR = has_colors();
	if (_COLOR) {
		start_color();
		init_pair(1, COLOR_WHITE, COLOR_BLACK); // fixes a bug with blinking text
	}

	splash = new Splash();

	maxHeight = getmaxy(window);
	maxWidth = getmaxx(window);
	for (int i = 0; i < MAX_BUFFERS; ++i)
		buffers[i] = new Buffer(maxWidth, maxHeight);

	active = false;
	debug = false;
	tickCount = 0;
}

Engine::~Engine() {
	active = false;
	for (int i = 0; i < MAX_BUFFERS; ++i)
		delete buffers[i];
	delete splash;
	endwin();
}

void Engine::Start(std::function< void(std::map< std::string, std::string >, Engine &) > fn) {
	loader = fn;
	active = true;
	loop();
}

void Engine::LoadContent(std::map< std::string, std::string > flags) { loader(flags, *this); }

void Engine::Stop() {
	delete level;
	active = false;
}

bool Engine::LoadLevel(Level *lvl) {
	level = lvl;
	return true;
}

std::pair< int, int > Engine::Direction(int up, int left, int down, int right) {
	std::pair< int, int > p = std::make_pair(0, 0);
	if (GetKey(right))
		p.first++;
	if (GetKey(left))
		p.first--;
	if (GetKey(down))
		p.second++;
	if (GetKey(up))
		p.second--;

	return p;
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

		if (GetKey(KEY_EXIT)) { // quit the game
			Stop();
			return;
		}

		GetCurBuffer()->ClearCanvas();
		if (splash->Active()) {
			splash->Tick(this);
			splash->Render(GetCurBuffer());
		} else {
			// Game logic and rendering
			tick();
			tickCount++;
			render();
		}
		drawBuffer();

		// Calculate sleep time for the rest of the frame, to save CPU load
		clock_t now = clock();
		double tm = double(now - drawTime) / CLOCKS_PER_SEC;
		double sleep = (1.0 / MAX_FPS - tm) * 1000000;
		if (sleep > 0)
			usleep(sleep);
		drawTime = clock();
	}
}

void Engine::tick() {
	if (GetKey(KEY_F(2)))
		debug = !debug;

	log << "tickCount: " << tickCount << std::endl; // DEBUG

	if (level && (level->GameState() == OK_STATE || level->GameState() == UNLOAD_STATE))
		level->Tick(this);
	else if (level && level->GameState() == LOOSE_STATE) {
		if (GetKey('\n')) {
			delete level;
			level = NULL;
			splash->Activate();
		}
	}
}

void Engine::render() {
	Buffer *buf = GetCurBuffer();

	if (level)
		level->Render(buf);

	if (debug)
		buf->DrawString(0, 0, log.str());
	log.str("");
}

void Engine::drawBuffer() {
	clear();
	erase();

	Buffer *buf = GetCurBuffer();
	attron(COLOR_PAIR(1));
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

SML::SML() {}
SML::~SML() {}

void SML::ReadFile(const std::string &file) {
	std::ifstream ifs;
	ifs.open(file.c_str());

	if (ifs.fail()) {
		std::cout << file << " error..." << std::endl;
		ifs.close();
		return;
	}

	std::cout << file << " succesfully opened..." << std::endl;

	char tmp[1024];
	int tmp_len = 0;
	std::stringstream ss;

	std::string key = "global";

	while (ifs.good()) {
		ss.str("");
		ifs.getline(tmp, 1024);
		tmp_len = strlen(tmp);

		while (tmp[tmp_len - 1] == '\\' && ifs.good()) {
			tmp[tmp_len - 1] = '\0';
			ss << tmp;
			ifs.getline(tmp, 1024);
			tmp_len = strlen(tmp);
		}
		ss << tmp;

		std::string line = ss.str();
		if (line[0] == '[' && line[line.size() - 1] == ']')
			key = line.substr(1, line.size() - 2);

		if (data.find(key) == data.end())
			data[key] = SML_Fragment();

		data[key].AddValue(line);
	}

	ifs.close();
}

void SML::ForEach(std::function< void(const std::string &, const SML_Fragment &) > f) const {
	for (const auto &v : data)
		if (v.first != "global")
			f(v.first, v.second);
}

const SML_Fragment &SML::GetFragment(const std::string &key) const { return data.find(key)->second; }

SML_Fragment::SML_Fragment() {}
SML_Fragment::~SML_Fragment() {}

void SML_Fragment::AddValue(const std::string &raw) {
	size_t index = raw.find(' ');
	data[raw.substr(0, index)] = raw.substr(index + 3);
}

std::string SML_Fragment::GetValue(const std::string &key) const { return data.find(key)->second; }
int SML_Fragment::GetValueAsInt(const std::string &key) const { return std::stoi(data.find(key)->second); }
char SML_Fragment::GetValueAsChar(const std::string &key) const { return data.find(key)->second[0]; }
short SML_Fragment::GetColor(const std::string &key) const {
	std::string val = data.find(key)->second;
	if (val == "WHITE")
		return COLOR_WHITE;
	else if (val == "GREEN")
		return COLOR_GREEN;
	else if (val == "MAGENTA")
		return COLOR_MAGENTA;
	else if (val == "BLUE")
		return COLOR_BLUE;
	else if (val == "BLACK")
		return COLOR_BLACK;
	else if (val == "YELLOW")
		return COLOR_YELLOW;
	else if (val == "CYAN")
		return COLOR_CYAN;
	else if (val == "RED")
		return COLOR_RED;
	else
		return COLOR_BLACK;
}

Splash::Splash() {
	logo = "                                                               \n"
		   "                                       ,-.----.                \n"
		   "                ,---,       ,-.----.   \\    /  \\    ,----..    \n"
		   "        ,---,  '  .' \\      \\    /  \\  |   :    \\  /   /   \\   \n"
		   "       /_ ./| /  ;    '.    ;   :    \\ |   |  .\\ :|   :     :  \n"
		   " ,---, |  ' ::  :       \\   |   | .\\ : .   :  |: |.   |  ;. /  \n"
		   "/___/ \\.  : |:  |   /\\   \\  .   : |: | |   |   \\ :.   ; /--`   \n"
		   " .  \\  \\ ,' '|  :  ' ;.   : |   |  \\ : |   : .   /;   | ;  __  \n"
		   "  \\  ;  `  ,'|  |  ;/  \\   \\|   : .  / ;   | |`-' |   : |.' .' \n"
		   "   \\  \\    ' '  :  | \\  \\ ,';   | |  \\ |   | ;    .   | '_.' : \n"
		   "    '  \\   | |  |  '  '--'  |   | ;\\  \\:   ' |    '   ; : \\  | \n"
		   "     \\  ;  ; |  :  :        :   ' | \\.':   : :    '   | '/  .' \n"
		   "      :  \\  \\|  | ,'        :   : :-'  |   | :    |   :    /   \n"
		   "       \\  ' ;`--''          |   |.'    `---'.|     \\   \\ .'    \n"
		   "        `--`                `---'        `---`      `---`      \n"
		   "                                                               ";

	menu.push_back("New game");
	menu.push_back("Quit");
	menuSelection = 0;
	menuStage = 0;
	active = true;

	flags["playerType"] = "";
	flags["playerName"] = "";
}

Splash::~Splash() {}

void Splash::Tick(Engine *engine) {
	if (menuStage == 0) {
		if (engine->GetKey(KEY_DOWN))
			menuSelection++;
		if (engine->GetKey(KEY_UP))
			menuSelection--;
		menuSelection = MIN(menuSelection, menu.size() - 1);

		if (engine->GetKey('\n')) {
			if (menuSelection == 0) {
				menuStage++;
			} else if (menuSelection == 1) {
				engine->Stop();
			}
		}
	} else if (menuStage == 1) {
		if (engine->GetKey('\n'))
			menuStage++;
		if (engine->GetKey(KEY_BACKSPACE) && flags["playerName"].size() > 0)
			flags["playerName"] = flags["playerName"].substr(0, flags["playerName"].size() - 1);

		for (int i = 65; i <= 90; ++i) {
			if (engine->GetKey(i))
				flags["playerName"] += (char)i;
		}
		for (int i = 97; i <= 122; ++i) {
			if (engine->GetKey(i))
				flags["playerName"] += (char)i;
		}
	} else if (menuStage == 2) {
		if (engine->GetKey('m') || engine->GetKey('M')) {
			flags["playerType"] = "melee";
			menuStage++;
		} else if (engine->GetKey('r') || engine->GetKey('R')) {
			flags["playerType"] = "ranged";
			menuStage++;
		}
	} else if (menuStage == 3) {
		active = false;
		flags["levelName"] = "level_1";
		engine->LoadContent(flags);
	}
}

void Splash::Render(Buffer *buffer) const {
	int logoWidth = 64;
	int logoHeight = 16;
	int startX = buffer->GetWidth() / 2 - logoWidth / 2;
	int startY = 5;

	buffer->DrawString(startX, startY, logo);

	if (menuStage == 0) {
		for (unsigned int i = 0; i < menu.size(); ++i) {
			int len = menu[i].size();
			if (menuSelection == i)
				buffer->DrawString(buffer->GetWidth() / 2 - len / 2 - 2, startY + logoHeight + 10 + i, "> " + menu[i]);
			else
				buffer->DrawString(buffer->GetWidth() / 2 - len / 2, startY + logoHeight + 10 + i, menu[i]);
		}
	} else if (menuStage == 1) {
		std::string text = "Choose your name: " + flags.find("playerName")->second;
		buffer->DrawString(buffer->GetWidth() / 2 - text.size() / 2, startY + logoHeight + 10, text);
	} else if (menuStage == 2) {
		std::string text = "Press [m] key for melee hero, or [r] key for ranged hero.";
		buffer->DrawString(buffer->GetWidth() / 2 - text.size() / 2, startY + logoHeight + 10, text);
	}
}

std::string GetPath() {
	char pBuf[255];
	short len = 255;

	char szTmp[32];
	sprintf(szTmp, "/proc/%d/exe", getpid());
	int bytes = MIN(readlink(szTmp, pBuf, len), len - 1);
	if (bytes >= 0)
		pBuf[bytes] = '\0';

	for (int i = bytes; i > 0; --i) {
		if (pBuf[i] == '/')
			break;
		else
			pBuf[i] = '\0';
	}

	std::string path(pBuf);
	return path;
}

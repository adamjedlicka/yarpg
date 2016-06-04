#ifndef __ENGINE_H
#define __ENGINE_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <unistd.h>

#define MAX_FPS 30	// Maximum FPS. Used to calculate sleep time between frames
#define MAX_KEYS 512  // Maximum number of keys ncurses can return
#define MAX_BUFFERS 1 // Maximum number of screen buffers engine can handle

class Drawable;
class Entity;
class Level;

class Buffer;

class Engine {
  private:
	WINDOW *window;
	std::ofstream log;
	bool keys[MAX_KEYS];

	uint16_t maxWidth, maxHeight;
	bool active;

	Buffer *buffers[MAX_BUFFERS];
	Level *level;

  public:
	Engine();
	~Engine();

	void Start();
	void Stop();

	bool LoadLevel(Level *);

	Buffer *GetCurBuffer();

  private:
	void loop();
	void tick();
	void render();
};

class Buffer {
  private:
	char **canvas;
	uint16_t width, height;

  public:
	Buffer();
	Buffer(uint16_t, uint16_t);
	~Buffer();

	void DrawChar(uint16_t, uint16_t, char);
	void ClearCanvas();

	uint16_t GetWidth() const { return this->width; };
	uint16_t GetHeight() const { return this->height; };
	char GetChar(uint16_t h, uint16_t w) const { return this->canvas[h][w]; };
	const char *GetLine(uint16_t h) const { return this->canvas[h]; };
};

class Drawable {
  public:
	virtual void Tick(Engine *) = 0;
	virtual void Render(Buffer *) const = 0;
};

class Entity : public Drawable {};

class Level : public Drawable {};

#endif

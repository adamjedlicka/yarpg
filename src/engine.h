#ifndef __ENGINE_H
#define __ENGINE_H

#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include <ncurses.h>

#define MAX_FPS 30	// Maximum FPS. Used to calculate sleep time between frames
#define MAX_KEYS 512  // Maximum number of keys ncurses can return
#define MAX_BUFFERS 1 // Maximum number of screen buffers engine can handle

class Drawable;
class Level;
class Entity;
class Structure;

class Buffer;

class Engine {
  private:
	WINDOW *window;
	std::ostringstream *log;
	bool keys[MAX_KEYS];

	int maxWidth, maxHeight;
	bool active, debug;
	int tickCount;

	Buffer *buffers[MAX_BUFFERS];
	Level *level;

  public:
	Engine();
	~Engine();

	void Start();
	void Stop();

	bool LoadLevel(Level *);

	Buffer *GetCurBuffer();

	bool GetKey(int i) { return this->keys[i]; };
	void SetLogStream(std::ostringstream *oss) { this->log = oss; };

  private:
	void loop();
	void tick();
	void render();
};

class Buffer {
  private:
	char **canvas;
	int width, height;

  public:
	Buffer();
	Buffer(int, int);
	~Buffer();

	void DrawChar(int, int, char);
	void DrawString(int, int, const std::string &);
	void ClearCanvas();

	int GetWidth() const { return this->width; };
	int GetHeight() const { return this->height; };
	char GetChar(int h, int w) const { return this->canvas[h][w]; };
	const char *GetLine(int h) const { return this->canvas[h]; };
};

class Drawable {
  public:
	virtual void Tick(Engine *) = 0;
	virtual void Render(Buffer *) const = 0;
};

class Entity : public Drawable {
  public:
	virtual Level *GetLevel() const = 0;
	virtual int GetPosX() const = 0;
	virtual int GetPosY() const = 0;
	virtual void SetLevel(Level *) = 0;
	virtual void SetXY(int, int) = 0;
};

class Level : public Drawable {
  public:
	virtual int GetOffX() const = 0;
	virtual int GetOffY() const = 0;
};

class Structure : public Drawable {
  public:
	virtual Level *GetLevel() const = 0;
	virtual int GetPosX() const = 0;
	virtual int GetPosY() const = 0;
	virtual void SetLevel(Level *) = 0;
	virtual void SetXY(int, int) = 0;
};

#endif

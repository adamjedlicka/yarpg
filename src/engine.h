#ifndef __ENGINE_H
#define __ENGINE_H

#include <cstdint>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include <ncurses.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

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
	bool keys[MAX_KEYS];

	int maxWidth, maxHeight;
	bool active, debug;
	int tickCount;

	Buffer *buffers[MAX_BUFFERS];
	Level *level;

  public:
	bool _COLOR;

	std::ostringstream log;

	Engine();
	~Engine();

	void Start();
	void Stop();

	bool LoadLevel(Level *);

	std::pair< int, int > Direction(int, int, int, int);

	Buffer *GetCurBuffer();
	bool GetKey(int i) { return keys[i]; };
	Level *GetCurLevel() { return level; };

  private:
	void loop();
	void tick();
	void render();
};

struct Cell {
	char ch;
	short fg, bg;
};

class Buffer {
  private:
	Cell **canvas;
	int width, height;

  public:
	Buffer();
	Buffer(int, int);
	~Buffer();

	void DrawChar(int, int, char);				 // position & char
	void DrawChar(int, int, char, short);		 // position, char and foreground color
	void DrawChar(int, int, char, short, short); // position, char, foreground and background color
	void DrawString(int, int, const std::string &);
	void ClearCanvas();

	int GetWidth() const { return width; };
	int GetHeight() const { return height; };
	char GetChar(int h, int w) const { return canvas[h][w].ch; };
	const Cell *GetLine(int h) const { return canvas[h]; };
};

class SML_Fragment {
  private:
	std::map< std::string, std::string > data;

  public:
	SML_Fragment();
	~SML_Fragment();

	void AddValue(const std::string &);
	std::string GetValue(const std::string &) const;
	int GetValueAsInt(const std::string &) const;
	char GetValueAsChar(const std::string &) const;
	short GetColor(const std::string &) const;
};

class SML {
  private:
	std::map< std::string, SML_Fragment > data;

  public:
	SML();
	~SML();

	void ReadFile(const std::string &);
	const SML_Fragment &GetFragment(const std::string &) const;
	void ForEach(std::function< void(const std::string &, const SML_Fragment &) >) const;
};

class Drawable {
  public:
	virtual ~Drawable(){};

	virtual void Tick(Engine *){};
	virtual void Render(Buffer *) const {};
};

class Entity : public Drawable {
  protected:
	Level *level;
	int posX, posY, dirX, dirY;
	bool destroyed;
	int hp, energy;

  public:
	Entity(int x, int y) { posX = x, posY = y, destroyed = false; };
	virtual ~Entity(){};

	virtual void Colide(Entity *){};
	virtual void Colide(Structure *){};
	virtual bool IsPlayer() { return false; };
	virtual bool IsSolid() { return false; };
	virtual void OnDestroy(){};
	virtual bool Attack(int) { return false; };

	std::pair< int, int > GetPos() { return std::make_pair(posX, posY); };
	void SetPos(int x, int y) { posX = x, posY = y; }
	std::pair< int, int > GetDir() { return std::make_pair(dirX, dirY); };
	void SetDir(int x, int y) { dirX = x, dirY = y; }
	Level *GetLevel() { return level; };
	void SetLevel(Level *lvl) { level = lvl; };
	bool Destroyed() { return destroyed; }
	void Destroy() {
		if (!destroyed) {
			OnDestroy();
			destroyed = true;
		}
	};
};

class Level : public Drawable {
  protected:
	int offX, offY;

  public:
	Level(){};
	virtual ~Level(){};

	virtual void SpawnEntity(Entity *) = 0;

	std::pair< int, int > GetOff() { return std::make_pair(offX, offY); };
};

class Structure : public Drawable {
  protected:
	Level *level;
	int posX, posY;
	bool destroyed;

  public:
	Structure(Level *lvl, int x, int y) { level = lvl, posX = x, posY = y; };
	virtual ~Structure(){};

	virtual void Colide(Entity *){};
	virtual bool IsSolid() { return true; };

	std::pair< int, int > GetPos() { return std::make_pair(posX, posY); };
	void SetPos(int x, int y) { posX = x, posY = y; }
	Level *GetLevel() { return level; };
	void SetLevel(Level *lvl) { level = lvl; };
	bool Destroyed() { return destroyed; }
	void Destroy() { destroyed = true; };
};

// helper functions
std::string GetPath();

#endif

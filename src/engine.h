#ifndef __Engine_H
#define __Engine_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <unistd.h>

#define MAX_FPS 30   // Maximum FPS. Used to calculate sleep time between frames
#define MAX_KEYS 512 // Maximum number of keys ncurses can return
#define MAX_BUFFERS 1

class Engine {
  private:
	WINDOW *window;
	std::ofstream log;
	bool keys[MAX_KEYS];

	uint16_t maxWidth, maxHeight;
	bool active;

	struct Buffer {
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
	Buffer *buffers[MAX_BUFFERS];

  public:
	Engine();
	~Engine();

	void Start();
	void Stop();

	Buffer *GetCurBuffer();

  private:
	void loop();
	void tick();
	void render();
};

#endif

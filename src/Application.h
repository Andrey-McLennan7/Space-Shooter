#pragma once

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "SDL2/SDL.h"

#define SCR_WIDTH 1280
#define SCR_HEIGHT 720
#define GLSL_VERSION "#version 330"

class Game;
class Font;
class Shader;

class Application
{
private:

	SDL_Window* m_window;

	Shader* m_model_shader;
	Shader* m_basic_shader;
	Shader* m_gui_shader;

	Game* m_game;

	Font* m_font;

	bool m_isRunning;

	friend Game;

public:

	Application();
	~Application();

	bool init();
	void loop();
	void close();
};

#endif
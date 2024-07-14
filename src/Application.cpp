#include "Application.h"

#include "Font.h"
#include "Game.h"
#include "Shader.h"

#include <iostream>

Application::Application() :
	m_window{ nullptr },
	m_game{ nullptr },
	m_font{ nullptr },
	m_isRunning{ true },
	m_model_shader{ nullptr },
	m_basic_shader{ nullptr },
	m_gui_shader{ nullptr }
{}

Application::~Application()
{
	this->close();
}

bool Application::init()
{
	/* Initialise Scene */
	// Create window
	this->m_window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	SDL_SetWindowResizable(this->m_window, SDL_FALSE);

	// Check window
	if (!this->m_window)
	{
		std::cerr << "ERROR:FAILED TO CREATE WINDOW" << std::endl;
		return false;
	}

	SDL_GL_CreateContext(this->m_window);

	// Check glew
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "ERROR:FAILED TO INITIALISE GLEW" << std::endl;
		return false;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Initialise font */
	// Construct font for GUI
	this->m_font = new Font{ "res/Fonts/Custom/batmfa.ttf" };

	// Check font import status
	if (!this->m_font->status())
	{
		std::cerr << "ERROR:" << this->m_font->error();
		return false;
	}

	/* Initialise shaders */
	// Construct vertex shader and fragment shader for the models
	this->m_model_shader = new Shader{ "res/Shader/vertexShader.glsl", "res/Shader/fragmentShader.glsl" };

	// Check shader compilation status
	if (!this->m_model_shader->status())
	{
		std::cerr << "ERROR:FAILED TO INITIALISE GAME : " << this->m_model_shader->error() << std::endl;
		return false;
	}

	// Construct vertex shader and fragment shader fpr the stars
	this->m_basic_shader = new Shader{ "res/Shader/basicVertexShader.glsl", "res/Shader/basicFragmentShader.glsl" };

	// Check shader compilation status
	if (!this->m_basic_shader->status())
	{
		std::cerr << "ERROR:FAILED TO INITIALISE GAME : " << this->m_basic_shader->error() << std::endl;
		return false;
	}

	// Construct vertex shader and fragment shader for the GUI
	this->m_gui_shader = new Shader{ "res/Shader/guiVertexShader.glsl", "res/Shader/guiFragmentShader.glsl" };

	// Check shader compilation status
	if (!this->m_gui_shader->status())
	{
		std::cerr << "ERROR:FAILED TO INITIALISE GAME : " << this->m_model_shader->error() << std::endl;
		return false;
	}

	/* Initialise Game */
	// Construct Game
	this->m_game = new Game{ this };

	// Check game construct status
	if (!this->m_game->status())
	{
		std::cerr << "ERROR:FAILED TO INITIALISE GAME : " << this->m_game->error() << std::endl;
		return false;
	}
	
	return true;
}

void Application::loop()
{
	while (this->m_isRunning)
	{
		this->m_game->frameRate();
		this->m_game->input();
		this->m_game->update();
		this->m_game->render();
	}
}

void Application::close()
{
	delete this->m_model_shader;
	delete this->m_basic_shader;
	delete this->m_gui_shader;
	delete this->m_font;
	delete this->m_game;

	SDL_DestroyWindow(this->m_window);
	SDL_Quit();
}
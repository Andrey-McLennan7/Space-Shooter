#include "Game.h"
#include "GUI.h"
#include "Star.h"
#include "Model.h"
#include "Player.h"
#include "Shader.h"
#include "Camera.h"
#include "Movement.h"
#include "Asteroid.h"
#include "BasicModel.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

float star_vertices[]
{
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f,

	-0.5f, -0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
};

Game::Game(Application* application) :
	m_application{ application },
	m_deltaTime{ 0.0f },
	m_tickCount{ 0 },
	m_player{ nullptr },
	m_player_model{ nullptr },
	m_asteroid_model{ nullptr },
	m_star_model{ nullptr },
	m_gui_player_distance{ nullptr },
	m_gui_player_health{ nullptr },
	m_gui_player_points{ nullptr },
	m_gui_player_overheating{ nullptr },
	m_camera{ nullptr }
{
	/* Initialise Player */
	// Import player model
	this->m_player_model = new Model{ "res/Objects/FA59AMako/FA59AMako.obj" };

	if (!this->m_player_model->status())
	{
		this->m_error_message = "FAILED TO IMPORT MODEL : PLAYER";
		this->m_success = false;
		return;
	}

	// Construct player
	this->m_player = new Player{ this, 25.0f, 3.0f };
	this->m_player->setModel(this->m_player_model);

	/* Initialise Asteroids */
	// Import asteroid model
	this->m_asteroid_model = new Model{ "res/Objects/Meteorite/NHMW-MIN-J2669-Nakhla_low_res.obj" };

	if (!this->m_asteroid_model->status())
	{
		this->m_error_message = "FAILED TO IMPORT MODEL : ASTEROID";
		this->m_success = false;
		return;
	}

	// Construct Asteroid
	for (int i{ 0 }; i < 20; ++i)
	{
		Asteroid* asteroid{ new Asteroid{ this, (float)(rand() % 6 + 5), 2.0f, "Asteroid_" + std::to_string(i + 1) } };

		int x{ 21 }, y{ 25 };
		switch (rand() % 2)
		{
		case 0:
			asteroid->setPostion(glm::vec3((float)(rand() % x + 60), (float)(rand() % y + 1), 0.0f));
			break;
		case 1:
			asteroid->setPostion(glm::vec3((float)(rand() % x + 60), -(float)(rand() % y + 1), 0.0f));
			break;
		}

		asteroid->setModel(this->m_asteroid_model);

		this->m_asteroids.push_back(asteroid);
	}

	/* Initialise Stars */
	this->m_star_model = new BasicModel{ star_vertices, sizeof(star_vertices) / sizeof(star_vertices[0]) };

	for (int i{ 0 }; i < 2000; ++i)
	{
		Star* star{ new Star{ this }};

		int x{ 110 }, y{ 200 };
		float z{ -49.9f };
		switch (rand() % 4)
		{
		case 0:
			star->setPosition(glm::vec3((float)(rand() % x), (float)(rand() % y), z));
			break;
		case 1:
			star->setPosition(glm::vec3(-(float)(rand() % x), (float)(rand() % y), z));
			break;
		case 2:
			star->setPosition(glm::vec3(-(float)(rand() % x), -(float)(rand() % y), z));
			break;
		case 3:
			star->setPosition(glm::vec3((float)(rand() % x), -(float)(rand() % y), z));
			break;
		}

		star->setScale(glm::vec3(0.5f));
		star->setModel(this->m_star_model);

		this->m_stars.push_back(star);
	}

	/* Initialise GUI */
	// Construct player distance GUI
	this->m_gui_player_distance = new GUI{ this };
	this->m_gui_player_distance->setFont(this->m_application->m_font);
	this->m_gui_player_distance->setScale(0.5f);

	// Construct player health GUI
	this->m_gui_player_health = new GUI{ this };
	this->m_gui_player_health->setFont(this->m_application->m_font);
	this->m_gui_player_health->setScale(0.5f);

	// Construct player points GUI
	this->m_gui_player_points = new GUI{ this };
	this->m_gui_player_points->setFont(this->m_application->m_font);
	this->m_gui_player_points->setScale(0.5f);

	// Construct player overheating GUI
	this->m_gui_player_overheating = new GUI{ this };
	this->m_gui_player_overheating->setFont(this->m_application->m_font);
	this->m_gui_player_overheating->setScale(0.5f);

	/* Initialise Camera */
	this->m_camera = new Camera{ glm::vec3(0.0f, 0.0f, 50.0f) };

	// Everything initialised successfully
	this->m_success = true;
}

Game::~Game()
{
	delete this->m_player_model;
	delete this->m_player;

	delete this->m_asteroid_model;
	for (std::vector<Asteroid*>::iterator itr{ this->m_asteroids.begin() }; itr != this->m_asteroids.end(); ++itr)
	{
		delete *itr;
	}

	delete this->m_star_model;
	for (std::vector<Star*>::iterator itr{ this->m_stars.begin() }; itr != this->m_stars.end(); ++itr)
	{
		delete *itr;
	}

	delete this->m_gui_player_distance;
	delete this->m_gui_player_health;

	delete this->m_camera;
}

void Game::frameRate()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks64(), this->m_tickCount + 16));

	// Delta time is the difference in ticks from last frame
	this->m_deltaTime = (SDL_GetTicks64() - this->m_tickCount) / 1000.0f;
	this->m_tickCount = SDL_GetTicks64();

	// Clamp maximum delta time value
	if (this->m_deltaTime > 0.05f)
	{
		this->m_deltaTime = 0.05f;
	}
}

void Game::input()
{
	// Read mouse input
	SDL_Event e;

	// Read keyboard input
	const Uint8* state{ SDL_GetKeyboardState(NULL) };

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT || state[SDL_SCANCODE_ESCAPE])
		{
			// Close Game
			this->m_application->m_isRunning = false;
		}
	}

	if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
	{
		// Move up
		this->m_player->processKeyboard(UP);
	}

	if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])
	{
		// Move down
		this->m_player->processKeyboard(DOWN);
	}

	if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
	{
		// Move left
		this->m_player->processKeyboard(LEFT);
	}

	if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
	{
		// Move right
		this->m_player->processKeyboard(RIGHT);
	}

	if (state[SDL_SCANCODE_SPACE])
	{
		// Shoot
		this->m_player->processKeyboard(SHOOT);
	}
}

void Game::update()
{
	/* Update Model Shader */
	// Vertex Shader
	glm::mat4 projection{ 1.0f };
	glm::mat4 view{ 1.0f };

	int width{ 0 }, height{ 0 };
	SDL_GetWindowSize(this->m_application->m_window, &width, &height);
	glViewport(0, 0, width, height);

	// Update projection matrix
	projection = glm::perspective(45.0f, float(width) / float(height), 0.1f, 100.0f);
	this->m_application->m_model_shader->setMat4("projection", projection);

	// Update view matrix
	view = this->m_camera->getViewMatrix();
	this->m_application->m_model_shader->setMat4("view", view);

	// Fragment Shader
	glm::vec3 lightPos{ glm::vec3{ 0.0f, 100.0f, 0.0f } };
	
	// Camera Object
	this->m_application->m_model_shader->setVec3("viewPos", this->m_camera->getPostion());

	// Light
	this->m_application->m_model_shader->setVec3("light.position", lightPos);
	this->m_application->m_model_shader->setVec3("light.ambient", glm::vec3(0.8f));
	this->m_application->m_model_shader->setVec3("light.diffuse", glm::vec3(0.4f));
	this->m_application->m_model_shader->setVec3("light.specular", glm::vec3(0.5f));

	/* Update GUI Shader */
	// Vertex Shader
	this->m_application->m_basic_shader->setMat4("projection", projection);
	this->m_application->m_basic_shader->setMat4("view", view);

	/* Update Scene Objects */
	this->m_player->update();

	for (std::vector<Asteroid*>::iterator itr{ this->m_asteroids.begin() }; itr != this->m_asteroids.end(); ++itr)
	{
		(*itr)->update();
	}

	for (std::vector<Star*>::iterator itr{ this->m_stars.begin() }; itr != this->m_stars.end(); ++itr)
	{
		(*itr)->update();
	}

	/* Update GUI */
	// Update GUI distance
	this->m_gui_player_distance->update();
	this->m_gui_player_distance->setText("Distance: " + std::to_string(this->m_player->getDistance()));
	this->m_gui_player_distance->setPosition(glm::vec2(25.0f, height - 50.0f));

	// Update GUI health
	this->m_gui_player_health->update();
	this->m_gui_player_health->setText("Health: " + std::to_string((int)this->m_player->getHealth()));
	this->m_gui_player_health->setPosition(glm::vec2(25.0f, height - 100.0f));
	
	// Update GUI health
	this->m_gui_player_points->update();
	this->m_gui_player_points->setText("Points: " + std::to_string(this->m_player->getPoints()));
	this->m_gui_player_points->setPosition(glm::vec2(25.0f, height - 150.0f));

	// Update GUI overheating
	this->m_gui_player_overheating->update();

	if (this->m_player->getOverheating() <= 0.0f || this->m_player->getOverheating() >= 1.0f)
	{
		this->m_gui_player_overheating->setText("Overheating: " + std::to_string((int)this->m_player->getOverheating()));
	}
	else
	{
		this->m_gui_player_overheating->setText("Overheating: " + std::to_string(this->m_player->getOverheating()));
	}

	this->m_gui_player_overheating->setPosition(glm::vec2(25.0f, height - 200.0f));
}

void Game::render()
{
	/* Render Background */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	/* Draw Scene Objects */
	// Player
	this->m_player->draw(this->m_application->m_model_shader);

	// Asteroids
	for (std::vector<Asteroid*>::iterator itr{ this->m_asteroids.begin() }; itr != this->m_asteroids.end(); ++itr)
	{
		(*itr)->draw(this->m_application->m_model_shader);
	}

	// Stars
	for (std::vector<Star*>::iterator itr{ this->m_stars.begin() }; itr != this->m_stars.end(); ++itr)
	{
		(*itr)->draw(this->m_application->m_basic_shader);
	}

	// GUI
	this->m_gui_player_distance->draw(this->m_application->m_gui_shader);
	this->m_gui_player_health->draw(this->m_application->m_gui_shader);
	this->m_gui_player_points->draw(this->m_application->m_gui_shader);
	this->m_gui_player_overheating->draw(this->m_application->m_gui_shader);

	/* Swap Buffers */
	SDL_GL_SwapWindow(this->m_application->m_window);
}

Shader* Game::getShader(int n) const
{
	switch (n)
	{
	case 0:
		return this->m_application->m_basic_shader;
	case 1:
		return this->m_application->m_model_shader;
	case 2:
		return this->m_application->m_gui_shader;
	default:
		std::cerr << "ERROR:UNABLE TO RETRIEVE SHADER" << std::endl;
		break;
	}
}
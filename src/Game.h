#pragma once

#ifndef _GAME_H_
#define _GAME_H_

#include "Application.h"

#include <string>
#include <vector>

class Model;
class Star;
class GUI;
class Player;
class Asteroid;
class Camera;
class BasicModel;

class Game
{
private:

	Application* m_application;

	Uint32 m_tickCount;
	float m_deltaTime;

	Model* m_player_model;
	Player* m_player;

	std::vector<Asteroid*> m_asteroids;
	Model* m_asteroid_model;

	std::vector<Star*> m_stars;
	BasicModel* m_star_model;

	GUI* m_gui_player_distance;
	GUI* m_gui_player_health;
	GUI* m_gui_player_points;
	GUI* m_gui_player_overheating;

	Camera* m_camera;

	std::string m_error_message;
	bool m_success;

	friend Player;
	friend Asteroid;
	friend Star;
	friend GUI;

public:

	Game(Application* application);
	~Game();

	void frameRate();
	void input();
	void update();
	void render();

	std::string error() { return this->m_error_message; }
	bool status() { return this->m_success; }

	SDL_Window* getWindow() const { return this->m_application->m_window; }
	bool& getRunner()		const { return this->m_application->m_isRunning; }

	Shader* getShader(int n = 0) const;
};

#endif
#pragma once

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "glm/glm.hpp"
#include <string>
#include <vector>

class Game;
class Model;
class Shader;
class Bullet;
class Asteroid;
class BasicModel;

class Player
{
private:

	Game* m_game;

	glm::vec3 m_position;
	glm::mat4 m_model_transformer;

	Model* m_model;

	float m_speed;
	float m_distance;
	float m_bRadius;
	float m_health;
	float m_time;
	float m_overheating;

	bool m_shoot;

	const float m_shoot_rate;

	int m_points;

	std::string m_name;

	std::vector<Bullet*> m_bullets;
	BasicModel* m_bullet_model;

	friend Asteroid;
	friend Bullet;

	friend class Star;

	bool CheckCollision(Asteroid* asteroid);
	Bullet* ShootBullet();

public:

	Player(Game* game, float speed = 1.0f, float bRadius = 1.0f, float shoot_rate = 0.03f, glm::vec3 position = glm::vec3(0.0f), std::string name = "Player");
	~Player();

	void processKeyboard(enum Movement movement);
	void update(float deltaTime = 0.0f);
	void draw(Shader* shader);

	// Setters
	void setPostion(glm::vec3 pos)	  { this->setPostion(pos.x, pos.y); }
	void setPostion(float x, float y) { this->m_position = glm::vec3(x, y, 0.0f);	}
	void setModel(Model* model)		  { this->m_model = model; }

	// Getters
	float getDistance()    const { return this->m_distance;    }
	float getHealth()	   const { return this->m_health;      }
	float getOverheating() const { return this->m_overheating; }
	int   getPoints()      const { return this->m_points;      }
};

#endif
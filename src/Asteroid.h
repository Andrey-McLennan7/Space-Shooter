#pragma once

#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include "glm/glm.hpp"
#include <string>

class Game;
class Model;
class Shader;

class Asteroid
{
private:

	glm::vec3 m_position;
	glm::mat4 m_model_transformer;

	Model* m_model;
	Game* m_game;

	float m_speed;
	float m_rotate;
	float m_bRadius;

	std::string m_name;

	bool m_reset;

	friend class Player;

	bool CheckCollision();
	void CheckIfOffScreen();

public:

	Asteroid(Game* game, float speed = 1.0f, float bRadius = 1.0f, std::string name = "Asteroid", glm::vec3 position = glm::vec3(0.0f));
	~Asteroid();

	void update(float deltaTime = 0.0f);
	void draw(Shader* shader);

	// Setters
	void setPostion(glm::vec3 pos)    { this->setPostion(pos.x, pos.y);			  }
	void setPostion(float x, float y) { this->m_position = glm::vec3(x, y, 0.0f); }
	void setModel(Model* model)		  { this->m_model = model;					  }
	
	void reset(bool reset) { this->m_reset = reset; }

	// Getters
	glm::vec3 getPosition()		  const { return this->m_position; }
	float     getSpeed()		  const { return this->m_speed;    }
	bool	  getBoundingRadius() const { return this->m_bRadius;  }
};

#endif
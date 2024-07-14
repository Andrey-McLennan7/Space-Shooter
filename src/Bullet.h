#pragma once

#ifndef _BULLET_H_
#define _BULLET_H_

#include "glm/glm.hpp"
#include <vector>

class BasicModel;
class Shader;
class Player;
class Asteroid;

class Bullet
{
private:

	glm::vec3 m_position;
	glm::mat4 m_model_transformer;

	BasicModel* m_model;

	Player* m_player;
	std::vector<Asteroid*>* m_asteroids;

	float m_speed;
	float m_bRadius;
	float m_rotate;

	bool m_undraw;

	bool CheckCollision(Asteroid* asteroid);

public:

	Bullet(Player* player, std::vector<Asteroid*>* asteroid, glm::vec3 position = glm::vec3(0.0f), float speed = 1.0f, float bRadius = 1.0f);
	~Bullet();

	void update(float deltaTime = 0.0f);
	void draw(Shader* shader);

	void setModel(BasicModel* model) { this->m_model = model; }

	bool undraw() const { return this->m_undraw; }
};

#endif
#include "Bullet.h"

#include "Game.h"
#include "Player.h"
#include "Asteroid.h"
#include "Shader.h"
#include "BasicModel.h"

#include <glm/gtc/matrix_transform.hpp>

Bullet::Bullet(Player* player, std::vector<Asteroid*>* asteroids, glm::vec3 position, float speed, float bRadius) :
	m_player{ player }, m_asteroids{ asteroids },
	m_position{ position }, m_speed{ speed },
	m_model_transformer{ glm::mat4(1.0f) },
	m_model{ nullptr }, m_bRadius{ bRadius },
	m_undraw{ false }, m_rotate{ 0.0f }
{}

Bullet::~Bullet()
{}

void Bullet::update(float deltaTime)
{
	this->m_rotate += 90.0f * deltaTime;

	this->m_position.x += this->m_speed * deltaTime;

	if (this->m_position.x >= 60.0f)
	{
		this->m_undraw = true;
	}

	for (std::vector<Asteroid*>::iterator itr{ this->m_asteroids->begin() }; itr != this->m_asteroids->end(); ++itr)
	{
		if (this->CheckCollision(*itr))
		{
			// Update player score
			if ((*itr)->getSpeed() <= 7.0f)
			{
				++this->m_player->m_points;
			}
			else
			{
				this->m_player->m_points += 2;
			}

			this->m_undraw = true;
			(*itr)->reset(true);
		}
	}

	// Update model matrix
	this->m_model_transformer = glm::mat4(1.0f);
	this->m_model_transformer = glm::translate(this->m_model_transformer, this->m_position);
	this->m_model_transformer = glm::rotate(this->m_model_transformer, glm::radians(this->m_rotate), glm::vec3(1.0f, 1.0f, 0.0f));
}

void Bullet::draw(Shader* shader)
{
	shader->setMat4("model", this->m_model_transformer);
	shader->setVec3("uColour", glm::vec3(0.0f, 128.0f / 255.0f, 1.0f));

	this->m_model->draw(shader);
}

bool Bullet::CheckCollision(Asteroid* asteroid)
{
	if (glm::length(asteroid->getPosition() - this->m_position) <= (this->m_bRadius + asteroid->getBoundingRadius()))
	{
		return true;
	}

	return false;
}
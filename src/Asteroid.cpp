#include "Asteroid.h"
#include "Player.h"
#include "Game.h"
#include "Model.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>

Asteroid::Asteroid(Game* game, float speed, float bRadius, std::string name, glm::vec3 position) :
	m_position{ position },
	m_model{ nullptr },
	m_reset{ false },
	m_game{ game },
	m_name{ name },
	m_speed{ speed },
	m_bRadius{ bRadius },
	m_rotate{ 0.0f },
	m_model_transformer{ glm::mat4{ 0.0f } }
{}

Asteroid::~Asteroid()
{}

void Asteroid::update(float deltaTime)
{
	deltaTime = this->m_game->m_deltaTime;

	this->m_position.x -= this->m_speed * deltaTime;

	this->CheckIfOffScreen();

	if (this->m_reset)
	{
		this->m_position.x = (float)(rand() % 21 + 60);
		this->m_speed = (float)(rand() % 6 + 5);
		this->m_reset = false;
	}

	if (this->m_speed > 7.0f)
	{
		this->m_rotate += 45.0f * deltaTime;
	}
	else
	{
		this->m_rotate += 90.0f * deltaTime;
	}

	// Update model matrix for asteroid
	this->m_model_transformer = glm::mat4{ 1.0f };
	this->m_model_transformer = glm::translate(this->m_model_transformer, this->m_position);
	this->m_model_transformer = glm::rotate(this->m_model_transformer, glm::radians(this->m_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	this->m_model_transformer = glm::scale(this->m_model_transformer, glm::vec3(0.05f));
}

void Asteroid::draw(Shader* shader)
{
	shader->setMat4("model", this->m_model_transformer);
	this->m_model->draw(shader);
}

bool Asteroid::CheckCollision()
{
	if (glm::length(this->m_position - this->m_game->m_player->m_position) <= (this->m_bRadius + this->m_game->m_player->m_bRadius))
	{
		return true;
	}

	return false;
}

void Asteroid::CheckIfOffScreen()
{
	if (this->m_position.x <= -60.0f)
	{
		int x{ 21 };
		int y{ 25 };

		x = rand() % x + 60;

		switch (rand() % 2)
		{
		case 0:
			y = rand() % y + 1;
			break;
		case 1:
			y = -(rand() % y + 1);
			break;
		}

		this->m_position.x = (float)x;
		this->m_position.y = (float)y;

		this->m_speed = (float)(rand() % 6 + 5);
	}
}
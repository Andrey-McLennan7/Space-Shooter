#include "Player.h"
#include "Game.h"
#include "Asteroid.h"
#include "Model.h"
#include "Movement.h"
#include "Bullet.h"
#include "BasicModel.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Player::Player(Game* game, float speed, float bRadius, float shoot_rate, glm::vec3 position, std::string name) :
	m_position{ position },
	m_model{ nullptr },
	m_game{ game },
	m_name{ name },
	m_speed{ speed },
	m_bRadius{ bRadius },
	m_health{ 100.0f },
	m_time{ shoot_rate },
	m_overheating{ 0.0f },
	m_shoot{ true },
	m_shoot_rate{ shoot_rate },
	m_points{ 0 },
	m_distance{ 0.0f },
	m_model_transformer{ glm::mat4{ 0.0f } }
{
	float bullet_vertices[] =
	{
		/*Top Position */
		-0.5f,-0.5f, 0.5f,
		 0.5f,-0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f,-0.5f, 0.5f,
		/* Bottom Position */
		-0.5f,-0.5f,-0.5f,
		 0.5f,-0.5f,-0.5f,
		 0.5f, 0.5f,-0.5f,
		 0.5f, 0.5f,-0.5f,
		-0.5f, 0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		/* Left Position */
		-0.5f,-0.5f,-0.5f,
		-0.5f, 0.5f,-0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f,-0.5f, 0.5f,
		-0.5f,-0.5f,-0.5f,
		/* Right Position */
		 0.5f,-0.5f,-0.5f,
		 0.5f, 0.5f,-0.5f,
		 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f,
		 0.5f,-0.5f, 0.5f,
		 0.5f,-0.5f,-0.5f,
		 /* Back Position */
		 -0.5f, 0.5f,-0.5f,
		  0.5f, 0.5f,-0.5f,
		  0.5f, 0.5f, 0.5f,
		  0.5f, 0.5f, 0.5f,
		 -0.5f, 0.5f, 0.5f,
		 -0.5f, 0.5f,-0.5f,
		 /* Front Position */
		 -0.5f,-0.5f,-0.5f,
		  0.5f,-0.5f,-0.5f,
		  0.5f,-0.5f, 0.5f,
		  0.5f,-0.5f, 0.5f,
		 -0.5f,-0.5f, 0.5f,
		 -0.5f,-0.5f,-0.5f,

	};

	this->m_bullet_model = new BasicModel{ bullet_vertices, sizeof(bullet_vertices) / sizeof(bullet_vertices[0]) };
}

Player::~Player()
{
	for (std::vector<Bullet*>::iterator itr{ this->m_bullets.begin() }; itr != this->m_bullets.end(); ++itr)
	{
		delete *itr;
	}

	delete this->m_bullet_model;
}

void Player::processKeyboard(enum Movement movement)
{
	float deltaTime{ this->m_game->m_deltaTime };

	glm::vec3 vertical{ glm::vec3(0.0f, this->m_speed, 0.0f) };
	glm::vec3 horizontal{ glm::vec3(this->m_speed, 0.0f, 0.0f) };

	if (UP == movement && this->m_position.y <= 25.0f)
	{
		// Move up
		this->m_position += vertical * deltaTime;
	}

	if (DOWN == movement && this->m_position.y >= -25.0f)
	{
		// Move down
		this->m_position -= vertical * deltaTime;
	}

	if (LEFT == movement && this->m_position.x >= -45.0f)
	{
		// Move left
		this->m_position -= horizontal * deltaTime;
	}

	if (RIGHT == movement && this->m_position.x <= 45.0f)
	{
		// Move right
		this->m_position += horizontal * deltaTime;
	}

	if (SHOOT == movement && this->m_shoot)
	{
		this->m_overheating += this->m_game->m_deltaTime * 0.5f;

		if (this->m_overheating >= 1.1f)
		{
			this->m_overheating = 1.1f;
			this->m_shoot = false;
		}

		if (this->m_time <= 0.0f)
		{
			// Shoot
			this->m_bullets.push_back(this->ShootBullet());

			// Restart timer
			this->m_time = this->m_shoot_rate;
		}
		else
		{
			// Decrease timer by delta time
			this->m_time -= this->m_game->m_deltaTime;
		}
	}
}

void Player::update(float deltaTime)
{
	deltaTime = this->m_game->m_deltaTime;

	if (this->m_overheating <= 0.0f)
	{
		this->m_overheating = 0.0f;
		this->m_shoot = true;
	}
	else
	{
		this->m_overheating -= this->m_game->m_deltaTime / 6.0f;
	}

	// Check collision with asteroid
	for (int i{ 0 }; i < this->m_game->m_asteroids.size(); ++i)
	{
		if (this->CheckCollision(this->m_game->m_asteroids.at(i)))
		{
			if (this->m_game->m_asteroids.at(i)->m_speed > 7.0f)
			{
				this->m_health -= 10;
			}
			else
			{
				this->m_health -= 5;
			}

			this->m_game->m_asteroids.at(i)->m_reset = true;
		}
	}

	// Check if bullet should be undrawn
	for (std::vector<Bullet*>::iterator itr{ this->m_bullets.begin()}; itr != this->m_bullets.end();)
	{
		if ((*itr)->undraw())
		{
			delete (*itr);
			itr = this->m_bullets.erase(itr);
		}
		else
		{
			++itr;
		}
	}

	// Update bullets
	for (std::vector<Bullet*>::iterator itr = this->m_bullets.begin(); itr != this->m_bullets.end(); ++itr)
	{
		(*itr)->update(deltaTime);
	}

	// Update distance
	this->m_distance += (this->m_speed / 10.0f) * deltaTime;

	// Check health condition
	if (this->m_health <= 0.00000001f)
	{
		system("cls");
		std::cout << "GAME OVER\n" << std::endl;
		system("pause");

		bool& run{ this->m_game->getRunner() };
		run = false;
	}

	// Update model matrix for player
	this->m_model_transformer = glm::mat4{ 1.0f };
	this->m_model_transformer = glm::translate(this->m_model_transformer, this->m_position);
	this->m_model_transformer = glm::rotate(this->m_model_transformer, glm::radians(90.0f), glm::vec3(0.0, 90.0f, 0.0f));
	this->m_model_transformer = glm::scale(this->m_model_transformer, glm::vec3(0.1f));
}

void Player::draw(Shader* shader)
{
	shader->setMat4("model", this->m_model_transformer);

	this->m_model->draw(shader);

	if (this->m_bullets.size() > 0)
	{
		for (std::vector<Bullet*>::iterator itr = this->m_bullets.begin(); itr != this->m_bullets.end(); ++itr)
		{
			(*itr)->draw(this->m_game->getShader(0));
		}
	}
}

bool Player::CheckCollision(Asteroid* asteroid)
{
	if (glm::length(asteroid->m_position - this->m_position) <= (this->m_bRadius + asteroid->m_bRadius))
	{
		return true;
	}

	return false;
}

Bullet* Player::ShootBullet()
{
	// Create bullet
	Bullet* bullet
	{
		new Bullet
		{
			this, &this->m_game->m_asteroids, this->m_position +
			glm::vec3(5.0f, 0.0f, 0.0f), 50.0f
		}
	};

	bullet->setModel(this->m_bullet_model);

	return bullet;
}
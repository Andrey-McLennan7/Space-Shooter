#include "Star.h"
#include "Game.h"
#include "Player.h"
#include "Shader.h"
#include "BasicModel.h"
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

Star::Star(Game* game) :
	m_position{ glm::vec3{ 0.0f } }, m_scale{ 1.0f },
	m_model_transformer{ glm::mat4{ 0.0f } },
	m_model{ nullptr }, m_game{ game }
{}

Star::~Star()
{}

void Star::update(float deltaTime)
{
	deltaTime = this->m_game->m_deltaTime;

	this->m_position.x -= (this->m_game->m_player->m_speed / 5.0f) * deltaTime;

	if (this->m_position.x <= -110.0f)
	{
		this->m_position.x = 110.0f;
	}

	// Update model matrix for star
	this->m_model_transformer = glm::mat4{ 1.0f };
	this->m_model_transformer = glm::translate(this->m_model_transformer, this->m_position);
	this->m_model_transformer = glm::scale(this->m_model_transformer, this->m_scale);
}

void Star::draw(Shader* shader)
{
	shader->setMat4("model", this->m_model_transformer);
	shader->setVec3("uColour", glm::vec3(1.0f));

	this->m_model->draw(shader);
}
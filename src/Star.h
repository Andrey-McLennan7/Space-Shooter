#pragma once

#ifndef _STAR_H_
#define _STAR_H

#include "glm/glm.hpp"

class Game;
class Shader;
class BasicModel;

class Star
{
private:

	Game* m_game;
	BasicModel* m_model;

	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::mat4 m_model_transformer;

public:

	Star(Game* game);
	~Star();

	void update(float deltaTime = 0.0f);
	void draw(Shader* shader);

	void setModel(BasicModel* model) { this->m_model = model; }

	void setPosition(glm::vec3 position) { this->m_position = position; }
	glm::vec3 getPosition() const { return this->m_position; }

	void setScale(glm::vec3 scale) { this->m_scale = scale; }
	glm::vec3 getScale() const { return this->m_scale; }
};

#endif
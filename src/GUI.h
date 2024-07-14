#pragma once

#ifndef _GUI_H_
#define _GUI_H_

#include <GL/glew.h>
#include "glm/glm.hpp"

#include <string>

class Font;
class Game;
class Shader;

class GUI
{
private:

	GLuint m_vao;
	GLuint m_vbo;

	Game* m_game;
	Font* m_font;

	glm::vec2 m_position;
	glm::mat4 m_projecion_transformer;

	float m_scale;

	std::string m_text;

public:

	GUI(Game* game, glm::vec2 position = glm::vec2(0.0f), float scale = 1.0f);
	~GUI();

	void update();
	void draw(Shader* shader);

	void setFont(Font* font) { this->m_font = font; }
	void setPosition(glm::vec2 position) { this->m_position = position; }
	void setScale(float scale) { this->m_scale = scale; }
	void setText(std::string text) { this->m_text = text; }
};

#endif
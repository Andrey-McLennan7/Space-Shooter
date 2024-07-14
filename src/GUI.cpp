#include "GUI.h"

#include "Font.h"
#include "Game.h"
#include "Shader.h"

#include "glm/gtc/matrix_transform.hpp"

GUI::GUI(Game* game, glm::vec2 position, float scale) :
	m_vao{ 0 }, m_vbo{ 0 },
	m_game{ game }, m_font{ nullptr },
	m_position{ position },
	m_scale{ scale },
	m_projecion_transformer{ glm::mat4(0.0f) }
{
	glGenBuffers(1, &this->m_vbo);
	glGenVertexArrays(1, &this->m_vao);

	glBindVertexArray(this->m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GUI::~GUI()
{}

void GUI::update()
{
	int width{ 0 }, height{ 0 };
	SDL_GetWindowSize(this->m_game->getWindow(), &width, &height);

	this->m_projecion_transformer = glm::mat4(1.0f);
	this->m_projecion_transformer = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
}

void GUI::draw(Shader* shader)
{
	shader->setMat4("projection", this->m_projecion_transformer);
	shader->setVec3("uTextColour", glm::vec3(191.0f / 255.0f));

	this->m_font->draw(this->m_vao, this->m_vbo, shader, this->m_text, this->m_position, this->m_scale);
}
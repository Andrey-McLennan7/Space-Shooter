#include "BasicModel.h"
#include "Shader.h"
#include <vector>

BasicModel::BasicModel(float* vertices, int size) :
	m_vao{ 0 }, m_vbo{ 0 }, m_size{ size / 3 }
{
	std::vector<float> data;

	for (int i{ 0 }; i < size; ++i)
	{
		data.push_back(vertices[i]);
	}

	glGenBuffers(1, &this->m_vbo);
	glGenVertexArrays(1, &this->m_vao);

	glBindVertexArray(this->m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data.at(0)) * data.size(), &data.at(0), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

BasicModel::~BasicModel()
{}

void BasicModel::draw(Shader* shader)
{
	shader->use();

	glBindVertexArray(this->m_vao);
	glDrawArrays(GL_TRIANGLES, 0, this->m_size);
	glBindVertexArray(0);

	shader->unuse();
}
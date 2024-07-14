#pragma once

#ifndef _BASIC_MODEL_H_
#define _BASIC_MODEL_H_

#include <GL/glew.h>

class Shader;

class BasicModel
{
private:

	GLuint m_vbo;
	GLuint m_vao;
	int m_size;

public:

	BasicModel(float* vertices, int size);
	~BasicModel();

	void draw(Shader* shader);
};

#endif
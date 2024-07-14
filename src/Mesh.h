#pragma once

#ifndef _MESH_H_
#define _MESH_H_

#include "Shader.h"

#include <glm/glm.hpp>

#include <vector>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	GLuint id;
	std::string type;
	std::string path;
};

class Mesh
{
private:

	// Render Data
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;

	// Mesh Data
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indecies;
	std::vector<Texture> m_textures;

	void SetupMesh();

public:

	// Constructors
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indicies, std::vector<Texture> textures);

	void Draw(Shader* shader);
};

#endif
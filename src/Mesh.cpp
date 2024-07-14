#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indicies, std::vector<Texture> textures) :
	m_vertices{ vertices }, m_indecies{ indicies }, m_textures{ textures }
{
	this->SetupMesh();
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &this->m_vao);
	glGenBuffers(1, &this->m_vbo);
	glGenBuffers(1, &this->m_ebo);

	glBindVertexArray(this->m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
	glBufferData(GL_ARRAY_BUFFER, this->m_vertices.size() * sizeof(Vertex), &this->m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_indecies.size() * sizeof(GLuint), &this->m_indecies[0], GL_STATIC_DRAW);

	// Position Attribute 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Normals Attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	// Texture Attribute 
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
}

void Mesh::Draw(Shader* shader)
{
	GLuint diffuseNr{ 1 };
	GLuint specularNr{ 1 };

	int index{ 0 };
	for (std::vector<Texture>::iterator itr{ this->m_textures.begin() }; itr != this->m_textures.end(); ++itr)
	{
		glActiveTexture(GL_TEXTURE0 + index);

		std::string name{ (*itr).type };
		std::string number;

		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}

		shader->setFloat(("materail." + name + number).c_str(), index);
		glBindTexture(GL_TEXTURE_2D, (*itr).id);

		++index;
	}

	glActiveTexture(GL_TEXTURE0);

	// Draw Mesh
	shader->use();
	glBindVertexArray(this->m_vao);
	glDrawElements(GL_TRIANGLES, this->m_indecies.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	shader->unuse();
}
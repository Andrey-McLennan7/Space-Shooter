#pragma once

#ifndef _MODEL_H_
#define _MODEL_H_

#include "Mesh.h"
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

GLuint TextureFromFile(const char* path, const std::string& directory);

class Model
{
private:

	// Model data
	std::vector<Texture> m_textures_loaded;
	std::vector<Mesh> m_meshes;
	std::string m_directory;

	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string typeName);

	bool m_success;

public:

	Model(const char* path);
	void draw(Shader* shader);

	bool status() const { return this->m_success; }
};

#endif
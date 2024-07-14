#include "Model.h"

#include "stb_image.h"

#include <iostream>

Model::Model(const char* path)
{
	this->LoadModel(path);
}

void Model::draw(Shader* shader)
{
	for (std::vector<Mesh>::iterator itr{ this->m_meshes.begin() }; itr != this->m_meshes.end(); ++itr)
	{
		(*itr).Draw(shader);
	}
}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene{ importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs) };

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		this->m_success = false;
		return;
	}

	this->m_directory = path.substr(0, path.find_last_of('/'));

	this->ProcessNode(scene->mRootNode, scene);

	this->m_success = true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// Process all the node's meshes
	for (int i{ 0 }; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh{ scene->mMeshes[node->mMeshes[i]] };
		this->m_meshes.push_back(this->ProcessMesh(mesh, scene));
	}

	// Then do the same for each of its children
	for (int i{ 0 }; i < node->mNumChildren; ++i)
		this->ProcessNode(node->mChildren[i], scene);
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	// Data
	std::vector<Vertex> vertices;
	std::vector<GLuint> indecies;
	std::vector<Texture> textures;

	// Vertex
	for (int i{ 0 }; i < mesh->mNumVertices; ++i)
	{
		// Position
		Vertex vertex;
		glm::vec3 vector3;

		vector3.x = mesh->mVertices[i].x;
		vector3.y = mesh->mVertices[i].y;
		vector3.z = mesh->mVertices[i].z;

		vertex.Position = vector3;

		// Normals
		if (mesh->HasNormals())
		{
			vector3.x = mesh->mNormals[i].x;
			vector3.y = mesh->mNormals[i].y;
			vector3.z = mesh->mNormals[i].z;

			vertex.Normal = vector3;
		}

		// Texture Coordinate
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vector2;

			vector2.x = mesh->mTextureCoords[0][i].x;
			vector2.y = mesh->mTextureCoords[0][i].y;

			vertex.TexCoords = vector2;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f);

		vertices.push_back(vertex);
	}

	// Indecies
	for (int i{ 0 }; i < mesh->mNumFaces; ++i)
	{
		aiFace face{ mesh->mFaces[i] };

		for (int j{ 0 }; j < face.mNumIndices; ++j)
			indecies.push_back(face.mIndices[j]);
	}

	// Texture
	aiMaterial* material{ scene->mMaterials[mesh->mMaterialIndex] };

	// Diffuse Map
	std::vector<Texture> diffuseMaps{ this->loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse") };
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	// Specular Map
	std::vector<Texture> specularMaps{ this->loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular") };
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	return Mesh(vertices, indecies, textures);
}

std::vector<Texture> Model::loadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;

	for (int i{ 0 }; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;

		mat->GetTexture(type, i, &str);

		bool skip{ false };

		for (int j{ 0 }; j < this->m_textures_loaded.size(); ++j)
		{
			if (std::strcmp(this->m_textures_loaded.at(j).path.data(), str.C_Str()) == 0)
			{
				textures.push_back(this->m_textures_loaded.at(j));
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture texture;

			texture.id = TextureFromFile(str.C_Str(), this->m_directory);
			texture.path = str.C_Str();
			texture.type = typeName;
			textures.push_back(texture);

			this->m_textures_loaded.push_back(texture);
		}
	}

	return textures;
}

GLuint TextureFromFile(const char* path, const std::string& directory)
{
	std::string fileName{ std::string(path) };
	fileName = directory + '/' + fileName;

	unsigned int textureID{ 0 };
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	/* Filter Options */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	int width{ 0 };
	int height{ 0 };
	int nrChannels{ 0 };

	unsigned char* data{ stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0) };

	if (data)
	{
		GLenum format{ 0 };

		switch (nrChannels)
		{
		case 1:
			format = GL_RED; // jpeg
			break;
		case 3:
			format = GL_RGB; // jpg
			break;
		case 4:
			format = GL_RGBA; // png
			break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}
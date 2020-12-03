#pragma once
#include "MeshTest.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

class Model
{
public:
	std::vector<Texture> texturesLoaded;
	std::vector<Mesh> meshes;
	bool gammaCorrection;

	Model(const std::string& path, bool gamma = false)
		:gammaCorrection(gamma)
	{
		LoadModel(path);
	}
	
	void Draw(Shader& shader)
	{

	}

private:
	std::string directory;
	void LoadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}

		directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene);
	}

	void ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProcessMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		for (unsigned int i = 0; mesh->mNumVertices; i++)
		{
			Vertex vertex;

			glm::vec3 vector;

			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;
			}

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 text;
				text.x = mesh->mTextureCoords[0][i].x;
				text.y = mesh->mTextureCoords[0][i].y;
				vertex.TextCoords = text;
			}
			else
			{
				vertex.TextCoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}
	}
};

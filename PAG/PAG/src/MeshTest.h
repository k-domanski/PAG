#pragma once
#include "Texture.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	VertexArray VAO;

	Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds, std::vector<Texture>& text)
		: vertices(verts), indices(inds), textures(text), VBO(VertexBuffer(verts)), IBO(IndexBuffer(inds, 36))
	{
		SetupMesh();
	}

	void Draw(Shader& shader)
	{
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			textures[i].Bind(i);
			shader.setUniform1i("u_Texture",i);
			shader.Unbind();
		}
		
	}

	inline const IndexBuffer& GetIBO() const { return IBO; }
private:
	VertexBuffer VBO;
	IndexBuffer IBO;
	VertexBufferLayout layout;

	void SetupMesh()
	{
		layout.Push<Vertex>(3);
		layout.Push<Vertex>(2);

		VAO.AddBuffer(VBO, layout);

		VAO.Unbind();
	}
};
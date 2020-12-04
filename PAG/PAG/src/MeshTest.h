#pragma once
#include "Texture.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <string>
#include <memory>

struct Text
{
	unsigned int id;
	std::string type;
	std::string path;
};
class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Text> textures;
	std::shared_ptr<VertexArray> VAO;

	Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds, std::vector<Text> text)
		: vertices(verts), indices(inds), textures(text)/*, VBO(VertexBuffer(verts)), IBO(IndexBuffer(inds, 36))*/
	{
		VBO = std::make_shared<VertexBuffer>(verts);
		//VBO = new VertexBuffer(verts);
		IBO = std::make_shared<IndexBuffer>(inds);
		VAO = std::make_shared<VertexArray>();
		SetupMesh();
	}
	

	void Draw(Shader& shader)
	{

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			std::string number;
			std::string name = textures[i].type;

			if (name == "texture_diffuse")
			{
				number = std::to_string(diffuseNr++);
			}
			else if (name == "texture_specular")
			{
				number = std::to_string(specularNr++);
			}
			else if (name == "texture_normal")
			{
				number = std::to_string(normalNr++);
			}
			else if (name == "texture_height")
			{
				number = std::to_string(heightNr++);
			}

			shader.setUniform1i((name + number),i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		
		//VAO->Bind();
		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		//VAO->Unbind();
		Renderer::Draw(*VAO, *IBO, shader);
		//glActiveTexture(GL_TEXTURE0);
	}

	inline const IndexBuffer& GetIBO() const { return *IBO; }
private:
	//VertexBuffer* VBO;
	std::shared_ptr<VertexBuffer> VBO;
	std::shared_ptr<IndexBuffer> IBO;
	VertexBufferLayout layout;

	void SetupMesh()
	{
		layout.Push<Vertex>(3);
		layout.Push<Vertex>(3);
		layout.Push<Vertex>(2);
		//layout.Push<Vertex>(3);
		//layout.Push<Vertex>(3);

		VAO->AddBuffer(*VBO, layout);

		VAO->Unbind();
	}
};
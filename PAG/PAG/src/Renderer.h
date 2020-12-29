#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


class Renderer
{
public:
	

	static void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader)
	{
		shader.Bind();
		vao.Bind();
		ibo.Bind();

		glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	static void Draw(const VertexArray& vao, const Shader& shader)
	{
		shader.Bind();
		vao.Bind();

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	static void Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};
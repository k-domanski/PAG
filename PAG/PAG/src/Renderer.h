#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


class Renderer
{
public:
	void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const;
	void Clear() const;
};
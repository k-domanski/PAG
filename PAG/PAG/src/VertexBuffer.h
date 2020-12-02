#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TextCoords;
};

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer() {}
	VertexBuffer(const std::vector<float>& data, unsigned int size);
	VertexBuffer(const std::vector<Vertex>& data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};
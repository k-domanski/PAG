//#include "Renderer.h"
//
//void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader)
//{
//	shader.Bind();
//	vao.Bind();
//	ibo.Bind();
//
//	glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
//}
//
//void Renderer::Clear()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}

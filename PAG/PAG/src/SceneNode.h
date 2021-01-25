#pragma once
#include <glm/glm.hpp>
#include "Model.h"
#include <glad/glad.h>


class SceneNode
{
public:

	SceneNode(glm::vec3 pos, glm::vec3 scale, Model& model, float rotSpeed, float angle)
		:_model(model), _rotationSpeed(rotSpeed)
	{
		_local = glm::translate(_local, pos);
		_local = glm::rotate(_local, angle, glm::vec3(1.0f, 0.0f, 1.0f));
		_local = glm::scale(_local, scale);
		//_radius = sqrtf((pos.x*pos.x) + (pos.z * pos.z));
	}

	SceneNode(glm::vec3 pos, glm::vec3 scale, VertexArray& _vao, float rotSpeed)
		: vao(_vao), _rotationSpeed(rotSpeed)
	{
		_local = glm::translate(_local, pos);
		_local = glm::rotate(_local, 45.0f, glm::vec3(1.0f, 0.0f, 1.0f));
		_local = glm::scale(_local, scale);
		//_radius = sqrtf((pos.x * pos.x) + (pos.z * pos.z));
	}

	SceneNode(glm::vec3 pos, glm::vec3 scale)
	{
		_local = glm::translate(_local, pos);
		_local = glm::scale(_local, scale);
	}

	void Draw(Shader& shader)
	{
		shader.SetUniformMat4f("model", _world);
		_model.Draw(shader);
		
		for (unsigned int i = 0; i < numOfChildren; i++)
		{
			children[i]->Draw(shader);
		}
	}

	void AddChild(SceneNode* child)
	{
		children.push_back(child);
		numOfChildren++;
	}

	void calculateWorld(glm::mat4 parrentWorld)
	{
		_world = parrentWorld * _local;

		for (unsigned int i = 0; i < numOfChildren; i++)
		{
			children[i]->calculateWorld(_world);
		}
	}

	//void DrawOrbits(VertexArray& VAO, Shader& shader, glm::mat4 parentWorld, int count)
	//{
	//	VAO.Bind();
	//	shader.Bind();
	//	glm::mat4 omodel = glm::mat4(1.0f);
	//	omodel = glm::scale(omodel, glm::vec3(_radius, 0.0f, _radius));
	//	shader.SetUniformMat4f("model", parentWorld * omodel);
	//	glDrawArrays(GL_LINE_LOOP, 0, count / 3);
	//	shader.Unbind();
	//	VAO.Unbind();
	//
	//	for (unsigned int i = 0; i < numOfChildren; i++)
	//	{
	//		children[i].DrawOrbits(VAO, shader, _world, count);
	//	}
	//}

	//void AddModel(std::vector<glm::mat4>& data)
	//{
	//	data.push_back(_world);
	//}

	inline glm::mat4& Local() { return _local; }
	inline glm::mat4& World() { return _world; }
	inline const int& NumOfChildren() const { return numOfChildren; }
	inline  std::vector<SceneNode*>& Children()  { return children; }

private:
	glm::mat4 _local;
	glm::mat4 _world;
	Model _model;
	//float _radius;
	std::vector<SceneNode*> children;
	int numOfChildren = 0;
	float _rotationSpeed;
	VertexArray vao;

	void RotateAroundParent()
	{
		glm::mat4 test = glm::rotate(glm::mat4(1.0f), _rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
		_local = test * _local;
	}
};
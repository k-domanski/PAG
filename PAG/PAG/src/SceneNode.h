#pragma once
#include <glm/glm.hpp>
#include "ModelTest.h"
#include <glad/glad.h>

struct Transform
{
	glm::mat4 Model;
	glm::mat4 newRot;

	Transform()
	{
		Model = glm::mat4(1.0f);
		newRot = glm::mat4(1.0f);
	}
};

class SceneNode
{
public:
	SceneNode(glm::vec3 pos, glm::vec3 scale, Model& model, float rotSpeed, float angle)
		:_model(model), isGeometry(false), _rotationSpeed(rotSpeed)
	{
		_local.Model = glm::translate(_local.Model, pos);
		_local.Model = glm::rotate(_local.Model, angle, glm::vec3(1.0f, 0.0f, 1.0f));
		_local.Model = glm::scale(_local.Model, scale);
	}

	SceneNode(glm::vec3 pos, glm::vec3 scale, Shader& shader, VertexArray& _vao, float rotSpeed)
		:geometry(shader), vao(_vao), isGeometry(true), _rotationSpeed(rotSpeed)
	{
		_local.Model = glm::scale(_local.Model, scale);
		_local.Model = glm::translate(_local.Model, pos);
	}

	void Draw(Shader& shader)
	{
		if (isGeometry)
		{
			vao.Bind();
			geometry.Bind();
			geometry.SetUniformMat4f("model1", _local.newRot * _world.Model);
			glDrawArrays(GL_POINTS, 0, 36);
			geometry.Unbind();
			vao.Unbind();
		}
		else
		{
			shader.SetUniformMat4f("model", _local.newRot * _world.Model);
			_model.Draw(shader);
		}
		
		
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

	void calculateWorld(SceneNode* currentNode, Transform parrentWorld, Transform parrentLocal)
	{
		glm::mat4 test = glm::rotate(glm::mat4(1.0), _rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
		_local.newRot = test * _local.newRot;
		_world.Model = parrentWorld.Model * _local.Model;
		for (unsigned int i = 0; i < numOfChildren; i++)
		{
			children[i]->calculateWorld(children[i], _world, _local);
		}
	}
	Transform _local;
	Transform _world;
	Model _model;
	std::vector<SceneNode*> children;
	int numOfChildren = 0;
	float _rotationSpeed;
	Shader geometry;
	VertexArray vao;
	bool isGeometry;
};
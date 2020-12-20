#pragma once
#include <glm/glm.hpp>
#include "Model.h"
#include <glad/glad.h>

struct Transform
{
	glm::mat4 Model;
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
		_radius = sqrtf((pos.x*pos.x) + (pos.z * pos.z));
	}

	SceneNode(glm::vec3 pos, glm::vec3 scale, Shader& shader, VertexArray& _vao, float rotSpeed)
		:geometry(shader), vao(_vao), isGeometry(true), _rotationSpeed(rotSpeed)
	{
		_local.Model = glm::translate(_local.Model, pos);
		_local.Model = glm::rotate(_local.Model, 45.0f, glm::vec3(1.0f, 0.0f, 1.0f));
		_local.Model = glm::scale(_local.Model, scale);
		_radius = sqrtf((pos.x * pos.x) + (pos.z * pos.z));
	}

	void Draw(Shader& shader)
	{
		if (isGeometry)
		{
			vao.Bind();
			geometry.Bind();
			geometry.SetUniformMat4f("model1", _world.Model);
			glDrawArrays(GL_POINTS, 0, 36);
			geometry.Unbind();
			vao.Unbind();
		}
		else
		{
			shader.SetUniformMat4f("model", _world.Model);
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
		RotateAroundParent();
		_world.Model = parrentWorld.Model * _local.Model;
		for (unsigned int i = 0; i < numOfChildren; i++)
		{
			children[i]->calculateWorld(children[i], _world, _local);
		}
	}

	void DrawOrbits(VertexArray& VAO, Shader& shader, Transform parentWorld, int count)
	{
		VAO.Bind();
		shader.Bind();
		glm::mat4 omodel = glm::mat4(1.0f);
		omodel = glm::scale(omodel, glm::vec3(_radius, 0.0f, _radius));
		shader.SetUniformMat4f("model", parentWorld.Model * omodel);
		glDrawArrays(GL_LINE_LOOP, 0, count / 3);
		shader.Unbind();
		VAO.Unbind();

		for (unsigned int i = 0; i < numOfChildren; i++)
		{
			children[i]->DrawOrbits(VAO, shader, _world, count);
		}
	}

	inline const Transform& Local() const{ return _local; }
	inline const Transform& World() const { return _world; }

private:
	Transform _local;
	Transform _world;
	Model _model;
	float _radius;
	std::vector<SceneNode*> children;
	int numOfChildren = 0;
	float _rotationSpeed;
	Shader geometry;
	VertexArray vao;
	bool isGeometry;

	void RotateAroundParent()
	{
		glm::mat4 test = glm::rotate(glm::mat4(1.0f), _rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
		_local.Model = test * _local.Model;
	}
};
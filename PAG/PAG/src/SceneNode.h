#pragma once
#include <glm/glm.hpp>
#include "ModelTest.h"
#include <glad/glad.h>

class SceneNode
{
public:
	SceneNode(glm::vec3 pos, glm::vec3 scale, Model& model)
		:_model(model), isGeometry(false)
	{
		_localScale = glm::scale(_localScale, scale);
		_localPosition = glm::translate(_localPosition, pos);
		
	}
	SceneNode(glm::vec3 pos, glm::vec3 scale, Shader& shader, VertexArray& _vao)
		:geometry(shader), vao(_vao), isGeometry(true)
	{
		_localScale = glm::scale(_localScale, scale);
		_localPosition = glm::translate(_localPosition, pos);
	}
	void Draw(Shader& shader)
	{
		if (isGeometry)
		{
			vao.Bind();
			geometry.Bind();
			geometry.SetUniformMat4f("model1", _worldPosition * _localScale);
			glDrawArrays(GL_POINTS, 0, 36);
			geometry.Unbind();
			vao.Unbind();
		}
		else
		{
			shader.SetUniformMat4f("model", _worldPosition * _localScale);
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

	void calculateWorld(SceneNode* currentNode, glm::mat4 parrentWorld)
	{
		_parentWorld = parrentWorld;
		glm::mat4 world = parrentWorld * currentNode->_localPosition;

		_worldPosition = world;
		for (unsigned int i = 0; i < numOfChildren; i++)
		{
			children[i]->calculateWorld(children[i],world);
		}
	}
	
	Model _model;
	std::vector<SceneNode*> children;
	glm::mat4 _localPosition = glm::mat4(1.0f);
	glm::mat4 _localScale = glm::mat4(1.0f);
	glm::mat4 _worldPosition = glm::mat4(1.0f);
	glm::mat4 _parentWorld = glm::mat4(1.0f);
	int numOfChildren = 0;
	Shader geometry;
	VertexArray vao;
	bool isGeometry;
};
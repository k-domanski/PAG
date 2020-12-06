#pragma once
#include <glm/glm.hpp>
#include "ModelTest.h"

class SceneNode
{
public:
	SceneNode(glm::vec3 pos, glm::vec3 scale, Model& model)
		:_model(model)
	{
		_localScale = glm::scale(_localScale, scale);
		_localPosition = glm::translate(_localPosition, pos);
		
	}

	void Draw(Shader& shader)
	{
		//glm::mat4 test = _localScale * _worldPosition;
		//_worldPosition = test;
		shader.SetUniformMat4f("model", _worldPosition * _localScale );
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

	void calculateWorld(SceneNode* currentNode, glm::mat4 parrentWorld)
	{
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
	int numOfChildren = 0;
	
};
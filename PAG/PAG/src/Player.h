#pragma once
#include "SceneNode.h"

enum PlayerDirection
{
	F,
	U,
	D,
	L,
	R
};


class Player
{
public:
	SceneNode* model;

	Player() {}
	void PlayerInput(PlayerDirection direction, float deltaTime)
	{
		float velocity = movementSpeed * deltaTime;
		glm::vec3 dir;
		if (direction == F)
		{
			dir = glm::vec3(0.0f, 0.0f, 1.0f);
			dir *= velocity;
			model->Local() = glm::translate(model->Local(), dir);
		}
		else if (direction == U)
		{
			dir = glm::vec3(0.0f, 1.0f, 0.0f);
			dir *= velocity;
			model->Local() = glm::translate(model->Local(), dir);
		}
		else if (direction == D)
		{
			dir = glm::vec3(0.0f, -1.0f, 0.0f);
			dir *= velocity;
			model->Local() = glm::translate(model->Local(), dir);
		}
		else if (direction == L)
		{
			model->Local() = glm::rotate(model->Local(), 2.0f * deltaTime, glm::vec3(0.0f,1.0f,0.0f));
		}
		else if (direction == R)
		{
			model->Local() = glm::rotate(model->Local(), -2.0f * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		}

	}

	void calculateMouseInput(float xoffset,float yoffset, float deltaTime)
	{
		previousYaw = Yaw;
		xoffset *= 0.1f;
		yoffset *= 0.1f;
		

		Yaw += xoffset;
		Pitch += yoffset;
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
		glm::vec3 front;
		//front.x = cos(glm::radians(Yaw));
		//front.z = sin(glm::radians(Yaw));
		//front.x = cos(glm::radians(Yaw)) * cos(glm::radians(0.0f));
		//front.y = sin(glm::radians(0.0f));
		//front.z = sin(glm::radians(Yaw)) * cos(glm::radians(0.0f));
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		//model->Local() = glm::rotate(model->Local(), front.x * deltaTime, glm::vec3(1.0f, 0.0f,0.0f));
	}

	inline const glm::vec3& getFront() const { return Front; }

	void rotateForward(float amount)
	{
		model->Local() = glm::rotate(model->Local(), amount/1000.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
private:
	float movementSpeed = 25.0f;
	glm::vec3 Front = glm::vec3(0.0f,0.0f,-1.0f);
	float Yaw = 90.0f;
	float Pitch = 0.0f;
	float previousYaw;
};
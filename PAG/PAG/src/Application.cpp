#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "SceneNode.h"
#include <math.h>


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera gCamera(glm::vec3(0.0f, 30.0f, 30.0f));
float gLastX = SCR_WIDTH / 2.0f;
float gLastY = SCR_HEIGHT / 2.0f;
bool gFirstMouse = true;

float gDeltaTime = 0.0f;
float gLastFrame = 0.0f;

struct drawingData
{
	std::vector<glm::vec3> newPositions;
	std::vector<glm::vec3> newScales;

	void clearData()
	{
		newPositions.clear();
		newScales.clear();
	}
};

void ProcessInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	{
		glEnable(GL_DEPTH_TEST);
		stbi_set_flip_vertically_on_load(true);
		
		/*3D house*/
		std::vector<float> quadVertices =
		{
			-0.05f,  0.05f,0.0f, 1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,0.0f, 0.0f, 1.0f, 0.0f,
			-0.05f, -0.05f,0.0f, 0.0f, 0.0f, 1.0f,
						   
			 0.05f,  0.05f,0.0f, 0.0f, 1.0f, 1.0f,

			 0.05f, -0.05f,-0.1f, 0.0f, 1.0f, 0.0f,
			 0.05f,  0.05f,-0.1f, 0.0f, 1.0f, 1.0f,

			 -0.05f, -0.05f,-0.1f, 0.0f, 0.0f, 1.0f,
			 -0.05f,  0.05f,-0.1f, 1.0f, 0.0f, 0.0f,
		};
		
		std::vector<unsigned int> houseIndices =
		{
			/*front quad*/
			0,1,2,
			0,1,3,

			/*right quad*/
			1, 3, 4,
			4, 5, 3,

			/*left quad*/
			0, 2, 6,
			0, 7, 6,

			/*back quad*/
			4,6,7,
			4,5,7,

			/*bottom quad*/
			2,6,1,
			1,4,6
		};


		IndexBuffer houseIbo(houseIndices);
		/*3D roof*/
		std::vector<float> triagleVertices =
		{
			-0.0f,  0.1f, -0.05f, 1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,0.0f, 0.0f, 1.0f, 0.0f,
			-0.05f, -0.05f,0.0f, 0.0f, 0.0f, 1.0f,

			 0.05f, -0.05f,-0.1f, 0.0f, 1.0f, 0.0f,
			 -0.05f, -0.05f,-0.1f, 0.0f, 0.0f, 1.0f
		};
		
		std::vector<unsigned int> roofIndices =
		{
			/*side triangles*/
			0, 1, 2,
			0, 1, 3,
			0, 2, 4,
			0, 3, 4,
			
			/*bottom quad*/
			1,2,3,
			1, 2, 4
		};
		IndexBuffer roofIbo(roofIndices);
		Shader shader("res/shaders/Basic.shader", 0);

		std::vector<glm::vec2> translations;
		float offset = 0.01f;
		for (int i = -10; i < 10; i += 2)
		{
			for (int j = -10; j < 10; j += 2)
			{
				glm::vec2 translation;
				translation.x = (float)i / 10.0f + offset;
				translation.y = (float)j / 10.0f + offset;
				translations.push_back(translation);
			}
		}
		VertexArray vao;
		VertexBuffer trans(translations);
		VertexBuffer vbo(triagleVertices, triagleVertices.size() * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);

		vao.AddBuffer(vbo, layout);
	
		SceneNode root(glm::vec3(0.0f), glm::vec3(1.0f));
		for (int i = -100; i < 100; i++)
		{
			for (int j = -100; j < 100; j++)
			{
				SceneNode test(glm::vec3((float)i, 0.0f, (float)j), glm::vec3(10.0f));
				root.AddChild(test);
			}
		}

		root.calculateWorld(root, root.World(), root.World());
		std::vector<glm::mat4> data;

		//for (unsigned int i = 0; i < translations.size(); i++)
		//{
		//	glm::mat4 model = glm::mat4(1.0f);
		//	model = glm::translate(model, glm::vec3(translations[i].x, translations[i].y, 0.0f));
		//	data.push_back(model);
		//}
		
		for (unsigned int i = 0; i < root.NumOfChildren(); i++)
		{
			data.push_back(root.Children()[i].World().Model);
		}

		VertexBuffer buffer(data);
		vao.Bind();
		buffer.Bind();
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		buffer.Unbind();
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		
		vao.Unbind();
		//vao.Bind();
		//glEnableVertexAttribArray(2);
		//glBindBuffer(GL_ARRAY_BUFFER, trans.GetID()); // this attribute comes from a different vertex buffer
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glVertexAttribDivisor(2, 1);

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 150");

		ImGui::StyleColorsDark();
		
		ImVec4 clear_color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

		bool isWireFrame = false;
		bool isPaused = false;
		int depth = 1;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			float currentFrame = glfwGetTime();
			gDeltaTime = currentFrame - gLastFrame;
			gLastFrame = currentFrame;

			ProcessInput(window);
			/* Render here */

			
			Renderer::Clear();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
			glm::mat4 view = gCamera.GetViewMatrix();
			shader.Bind();
			shader.SetUniformMat4f("projection", projection);
			shader.SetUniformMat4f("view", view);
			
			if (!isPaused)
			{
				
			}

			vao.Bind();
			roofIbo.Bind();
			//glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
			glDrawElementsInstanced(GL_TRIANGLES, roofIbo.GetCount(), GL_UNSIGNED_INT, nullptr, data.size());
			

			if (isWireFrame)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			{
				ImGui::Begin("Hello, world!");

				ImGui::Checkbox("Wireframe", &isWireFrame);
				ImGui::Checkbox("Pause", &isPaused);
				ImGui::SliderInt("Depth", &depth, 1, 20);
				ImGui::ColorEdit3("clear color", (float*)& clear_color);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
			
			

			
		}
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

void ProcessInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		gCamera.ProcessKeyboard(LEFT, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	gCamera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (gFirstMouse)
	{
		gLastX = xpos;
		gLastY = ypos;
		gFirstMouse = false;
	}

	float xoffset = xpos - gLastX;
	float yoffset = gLastY - ypos;

	gLastX = xpos;
	gLastY = ypos;

	gCamera.ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}
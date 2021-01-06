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
		//std::vector<float> houseVertices =
		//{
		//	-0.05f,  0.05f,0.0f, 1.0f, 0.0f, 0.0f,
		//	 0.05f, -0.05f,0.0f, 0.0f, 1.0f, 0.0f,
		//	-0.05f, -0.05f,0.0f, 0.0f, 0.0f, 1.0f,
		//				   
		//	 0.05f,  0.05f,0.0f, 0.0f, 1.0f, 1.0f,
		//
		//	 0.05f, -0.05f,-0.1f,0.0f, 1.0f, 0.0f,
		//	 0.05f,  0.05f,-0.1f,0.0f, 1.0f, 1.0f,
		//
		//	 -0.05f, -0.05f,-0.1f, 0.0f, 0.0f, 1.0f,
		//	 -0.05f,  0.05f,-0.1f, 1.0f, 0.0f, 0.0f,
		//};
		
		std::vector<float> houseVertices =
		{
			-0.05f, -0.05f, -0.05f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f, -0.05f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
			 0.05f,  0.05f, -0.05f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
			 0.05f,  0.05f, -0.05f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, //
			-0.05f,  0.05f, -0.05f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
			-0.05f, -0.05f, -0.05f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, //
							  
			-0.05f, -0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
			 0.05f,  0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
			 0.05f,  0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,  //
			-0.05f,  0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
			-0.05f, -0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,  //
							  
			-0.05f,  0.05f,  0.05f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			-0.05f,  0.05f, -0.05f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			-0.05f, -0.05f, -0.05f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			-0.05f, -0.05f, -0.05f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			-0.05f, -0.05f,  0.05f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			-0.05f,  0.05f,  0.05f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
							  
			 0.05f,  0.05f,  0.05f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			 0.05f,  0.05f, -0.05f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			 0.05f, -0.05f, -0.05f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			 0.05f, -0.05f, -0.05f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			 0.05f, -0.05f,  0.05f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			 0.05f,  0.05f,  0.05f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
							 
			-0.05f, -0.05f, -0.05f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
			 0.05f, -0.05f, -0.05f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			 0.05f, -0.05f,  0.05f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
			 0.05f, -0.05f,  0.05f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
			-0.05f, -0.05f,  0.05f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
			-0.05f, -0.05f, -0.05f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
							  
			-0.05f,  0.05f, -0.05f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,//
			 0.05f,  0.05f, -0.05f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			 0.05f,  0.05f,  0.05f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,//
			 0.05f,  0.05f,  0.05f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
			-0.05f,  0.05f,  0.05f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
			-0.05f,  0.05f, -0.05f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
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
		std::vector<unsigned int> groundInd =
		{
			31,33,34,
			34, 31, 35
		};
		IndexBuffer houseIbo(houseIndices);
		IndexBuffer groundIbo(groundInd);

		/*3D roof*/
		std::vector<float> roofVertices =
		{
			-0.0f,   0.1f, -0.05f, 0.0f, 0.5f, -0.15f, 0.5f, 1.0f,
			 0.05f, -0.05f, 0.0f,  0.0f, 0.5f, -0.15f, 1.0f, 0.0f,
			-0.05f, -0.05f, 0.0f,  0.0f, 0.5f, -0.15f, 0.0f, 0.0f,

			-0.0f,   0.1f, -0.05f, 0.15f, 0.5f, 0.0f, 0.5f, 1.0f,
			 0.05f, -0.05f, 0.0f,  0.15f, 0.5f, 0.0f, 1.0f, 0.0f,
			 0.05f, -0.05f,-0.1f,  0.15f, 0.5f, 0.0f, 0.0f, 0.0f,

			 -0.0f,   0.1f, -0.05f, 0.15f, 0.5f, 0.0f, 0.5f, 1.0f,
			 -0.05f, -0.05f, 0.0f,  0.15f, 0.5f, 0.0f,	1.0f, 0.0f,
			 -0.05f, -0.05f,-0.1f,  0.15f, 0.5f, 0.0f,	0.0f, 0.0f,

			 -0.0f,   0.1f, -0.05f, 0.0f, 0.5f, -0.15f, 0.5f, 1.0f,
			  0.05f, -0.05f,-0.1f,  0.0f, 0.5f, -0.15f,	1.0f, 0.0f,
			 -0.05f, -0.05f,-0.1f,  0.0f, 0.5f, -0.15f,	0.0f, 0.0f,

			 0.05f, -0.05f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			-0.05f, -0.05f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			 0.05f, -0.05f,-0.1f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
			 0.05f, -0.05f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
			-0.05f, -0.05f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
			-0.05f, -0.05f,-0.1f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		};

		//std::vector<float> roofVertices =
		//{
		//	-0.0f,  0.1f, -0.05f, 1.0f, 0.0f, 0.0f,
		//	 0.05f, -0.05f,0.0f, 0.0f, 1.0f, 0.0f,
		//	-0.05f, -0.05f,0.0f, 0.0f, 0.0f, 1.0f,
		//
		//	 0.05f, -0.05f,-0.1f, 0.0f, 1.0f, 0.0f,
		//	 -0.05f, -0.05f,-0.1f, 0.0f, 0.0f, 1.0f
		//};
		
		std::vector<unsigned int> roofIndices =
		{
			/*side triangles*/
			0, 1, 2,
			0, 1, 3,
			0, 2, 4,
			0, 3, 4,
			
			/*bottom quad*/
			1, 2, 3,
			1, 2, 4
		};
		
		//IndexBuffer roofIbo(roofIndices);
		
		
		Shader shader("res/shaders/Basic.shader", 0);
		Shader light("res/shaders/LightSource.shader", 1);

		Texture roofTexture("roof.png", "res/textures", "diffuse");
		roofTexture.Bind(0);
		Texture houseTexture("brickWall.jpg", "res/textures", "diffuse");
		houseTexture.Bind(0);
		Texture groundTexture("grass.jpg", "res/textures", "diffuse");
		groundTexture.Bind(0);
		roofTexture.Unbind();
		houseTexture.Unbind();
		groundTexture.Unbind();

		VertexArray lightVAO;

		VertexArray houseVAO;
		VertexArray roofVAO;
		VertexArray groundVao;
		VertexBuffer roofVBO(roofVertices, roofVertices.size() * sizeof(float));
		VertexBuffer houseVBO(houseVertices, houseVertices.size() * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);

		houseVAO.AddBuffer(houseVBO, layout);
		roofVAO.AddBuffer(roofVBO, layout);
		lightVAO.AddBuffer(houseVBO, layout);
		groundVao.AddBuffer(houseVBO, layout);
	
		SceneNode root(glm::vec3(0.0f), glm::vec3(1.0f));
		for (int i = -100; i < 101; i++)
		{
			for (int j = -100; j < 101; j++)
			{
				SceneNode test(glm::vec3((float)i, 0.0f, (float)j), glm::vec3(5.0f));
				SceneNode test1(glm::vec3(0.0f, 0.1f, 0.05f), glm::vec3(1.0f));
				test.AddChild(test1);
				root.AddChild(test);
			}
		}
		
		//root.World().Model = glm::rotate(root.World().Model, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		std::vector<glm::mat4> data;
		std::vector<glm::mat4> data1;
		std::vector<glm::mat4> data2;
		root.calculateWorld(root, root.World());

		for (unsigned int i = 0; i < root.NumOfChildren(); i++)
		{
			data.push_back(root.Children()[i].World().Model);
			for (int j = 0; j < root.Children()[i].NumOfChildren(); j++)
			{
				data1.push_back(root.Children()[i].Children()[j].World().Model);
			}
		}
		SceneNode lightS(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(5.0f, 50.0f, 50.0f));
		root.AddChild(lightS);
		SceneNode ground(glm::vec3(0.0f, -0.2f, 0.0f), glm::vec3(2500.0f,1.0f, 2500.0f));
		root.AddChild(ground);
		root.calculateWorld(root, root.World());
		data2.push_back(root.Children()[root.Children().size() - 1].World().Model);
		VertexBuffer roofBuffer(data1);
		VertexBuffer houseBuffer(data);
		VertexBuffer groundBuffer(data2);

		groundVao.Bind();
		groundBuffer.Bind();
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		groundBuffer.Unbind();
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		groundVao.Unbind();

		roofVAO.Bind();
		roofBuffer.Bind();
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		roofBuffer.Unbind();
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		roofVAO.Unbind();

		houseVAO.Bind();
		houseBuffer.Bind();
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		houseBuffer.Unbind();
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		houseVAO.Unbind();


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
			
			shader.SetUniformVec3f("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
			shader.SetUniformVec3f("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
			shader.SetUniformVec3f("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));

			//shader.SetUniformVec3f("light.direction", glm::vec3(1.0f, 1.0f, 1.0f));
			shader.SetUniformVec3f("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			shader.SetUniformVec3f("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
			shader.SetUniformVec3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
			shader.SetUniform1f("material.shininess", 64.0f);

			//shader.SetUniformVec3f("light.position", root.Children()[1].World().Model[3]);
			shader.SetUniformVec3f("light.position", gCamera.Position);
			shader.SetUniformVec3f("light.direction", gCamera.Front);
			shader.SetUniform1f("light.cutOff", glm::cos(glm::radians(12.5f)));
			shader.SetUniform1f("light.outerCutOff", glm::cos(glm::radians(17.5f)));
			shader.SetUniform1f("light.constant", 1.0f);
			shader.SetUniform1f("light._linear", 0.09f);
			shader.SetUniform1f("light.quadratic", 0.032f);
			//shader.SetUniformVec3f("lightPos", root.Children()[1].World().Model[3]);
			shader.SetUniformVec3f("viewPos", gCamera.Position);
			
			roofVAO.Bind();
			roofTexture.Bind(0);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 36, data1.size());
			//glDrawElementsInstanced(GL_TRIANGLES, roofIbo.GetCount(), GL_UNSIGNED_INT, nullptr, data.size());
			roofVAO.Unbind();
			roofTexture.Unbind();
			//roofIbo.Unbind();

			houseVAO.Bind();
			houseTexture.Bind(0);
			//houseIbo.Bind();
			glDrawArraysInstanced(GL_TRIANGLES, 0, 36, data.size());
			////glDrawElementsInstanced(GL_TRIANGLES, houseIbo.GetCount(), GL_UNSIGNED_INT, nullptr, data1.size());
			houseVAO.Unbind();
			houseTexture.Unbind();
			//houseIbo.Unbind();

			groundVao.Bind();
			groundTexture.Bind(0);
			//glDrawArraysInstanced(GL_TRIANGLES, 0, 36, data2.size());
			Renderer::Draw(groundVao, groundIbo, shader);
			groundTexture.Unbind();

			shader.Unbind();

			light.Bind();
			light.SetUniformMat4f("projection", projection);
			light.SetUniformMat4f("view", view);
			light.SetUniformMat4f("model", root.Children()[1].World().Model);
			//Renderer::Draw(lightVAO, light);
			//Renderer::Draw(lightVAO, houseIbo, light);
			light.Unbind();
			lightVAO.Unbind();

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
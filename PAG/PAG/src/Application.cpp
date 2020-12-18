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
#include "MeshTest.h"
#include "ModelTest.h"
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
		
		std::vector<float> points =
		{
			0.0f,  0.0f, 0.0f
		};
		
		VertexBuffer vboCillinder(points, points.size() * sizeof(float));
		VertexBufferLayout layout1;
		layout1.Push<float>(2);
		VertexArray vao;
		vao.AddBuffer(vboCillinder, layout1);
		Shader geometry("res/shaders/GeometryTest.shader", 2);
			
		std::vector<float> orbits;
		for (unsigned int i = 0; i < 2000; i++)
		{
			float angle = (float)(3.14f / 2.0f - i * (3.14f / 1000));
			float x = std::sinf(angle);
			float z = std::cosf(angle);
			orbits.push_back(x);
			orbits.push_back(0.0f);
			orbits.push_back(z);

		}
		

		VertexArray VAO;
		VertexBufferLayout layout;
		layout.Push<float>(3);
		VertexBuffer vbo(orbits, orbits.size()*sizeof(float));
		VAO.AddBuffer(vbo, layout);
		Shader test("res/shaders/Test.shader", 1);
		Shader shader("res/shaders/Basic.shader", 0);
		
		/*models init*/
		Model backpack("res/models/planets/moon2.obj");
		Model s("res/models/planets/sun/Sun1.obj");
		Model p1("res/models/planets/planet2.obj");
		Model p2("res/models/planets/planet3.obj");
		Model p3("res/models/planets/planet5.obj");
		Model p4("res/models/planets/planet6.obj");
		Model m1("res/models/planets/Moon.obj");
		Model m2("res/models/planets/moon1.obj");
		Model m3("res/models/planets/moon2.obj");
		Model m4("res/models/planets/planet1.obj");
		Model m5("res/models/planets/planet4.obj");
		Model m6("res/models/planets/planet4.obj");
		
		/*Scene graph init*/
		SceneNode sun(glm::vec3(0.0f), glm::vec3(1.0f), s, 0.01f, 0.0f);
		SceneNode planet1(glm::vec3(5.0f, 0.0f, 0.0f),glm::vec3(1.0f), p1, 0.01f, 30.0f);
		SceneNode planet2(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(1.0f), p2, 0.03f, 45.0f);
		SceneNode planet3(glm::vec3(15.0f, 0.0f, 0.0f), glm::vec3(1.0f), p3, 0.02f, 50.0f);
		SceneNode planet4(glm::vec3(20.0f, 0.0f, 0.0f), glm::vec3(1.0f), p4, 0.04f, 65.0f);
		SceneNode moon1(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.5f), m1, 0.02f, 75.0f);
		SceneNode moon2(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.5f), m2, 0.02f, 60.0f);
		SceneNode moon3(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.5f), m3, 0.02f, 35.0f);
		SceneNode moon4(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.5f), m4, 0.02f, 25.0f);
		SceneNode moon5(glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(0.5f), m5, 0.02f, 15.0f);
		SceneNode moon6(glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(0.25f), m6, 0.02f, 85.0f);
		SceneNode moon7(glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(0.25f), m4, 0.02f, 55.0f);
		SceneNode cylinder(glm::vec3(25.0f, 0.0f, 0.0f), glm::vec3(1.0f), geometry, vao, 0.02f);
		
		sun.AddChild(&planet1);
		sun.AddChild(&planet2);
		sun.AddChild(&planet3);
		sun.AddChild(&planet4);
		sun.AddChild(&cylinder);
		
		planet1.AddChild(&moon1);
		planet1.AddChild(&moon7);
		planet2.AddChild(&moon2);
		planet3.AddChild(&moon3);
		planet3.AddChild(&moon6);
		planet4.AddChild(&moon4);
		planet4.AddChild(&moon5);

		std::vector<SceneNode*> planets;
		planets.push_back(&planet1);
		planets.push_back(&planet2);
		planets.push_back(&planet3);
		planets.push_back(&planet4);
		planets.push_back(&cylinder);
		

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 150");

		ImGui::StyleColorsDark();
		
		ImVec4 clear_color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

		bool isWireFrame = false;
		int depth = 1;
		//sun._local.Position = glm::rotate(sun._local.Position, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 neewRot = glm::rotate(glm::mat4(1.0f), planet1._rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			float currentFrame = glfwGetTime();
			gDeltaTime = currentFrame - gLastFrame;
			gLastFrame = currentFrame;

			ProcessInput(window);
			/* Render here */

			//planet1._worldPosition = glm::rotate(planet1._worldPosition, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
			
			Renderer::Clear();
			shader.Bind();
			glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = gCamera.GetViewMatrix();
			geometry.Bind();
			geometry.SetUniformMat4f("projection1", projection);
			geometry.SetUniformMat4f("view1", view);
			geometry.setUniform1i("depth", depth);
			geometry.Unbind();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			shader.Bind();
			shader.SetUniformMat4f("projection", projection);
			shader.SetUniformMat4f("view", view);
			//for (auto& planet : planets)
			//{
			//	for (unsigned int i = 0; i < planet->numOfChildren; i++)
			//	{
			//
			//		planet->children[i]->_local.Model = glm::rotate(planet->children[i]->_local.Model, planet->children[i]->_rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
			//	}
			//	//glm::mat4 neewRot = glm::rotate(glm::mat4(1.0f), planet->_rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
			//	//planet->_local.Model = neewRot * planet->_local.Model;
			//	//planet->_local.Model = glm::rotate(planet->_local.Model, planet->_rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
			//}
			//planet1._local.Model = neewRot * planet1._local.Model;
			sun.calculateWorld(&sun, sun._world, sun._local);
			sun.Draw(shader);
			shader.Unbind();
			
			
			/*PLANET ORBITS*/
			//for (unsigned int i = 1; i < planets.size() +1; i++)
			//{
			//	VAO.Bind();
			//	test.Bind();
			//	glm::mat4 omodel = glm::mat4(1.0f);
			//	omodel = glm::scale(omodel, glm::vec3(5.0f * i, 0.0f, 5.0f * i));
			//	test.SetUniformMat4f("model", omodel);
			//	test.SetUniformMat4f("projection", projection);
			//	test.SetUniformMat4f("view", view);
			//	glDrawArrays(GL_LINE_LOOP, 0, orbits.size() / 3);
			//	test.Unbind();
			//	VAO.Unbind();
			//}
			
			///*MOON ORBITS*/
			for (auto& planet : planets)
			{
				VAO.Bind();
				test.Bind();
				glm::mat4 omodel = glm::mat4(1.0f);
				omodel = glm::scale(omodel, glm::vec3(planet->_local.Model[3].x, 0.0f, planet->_local.Model[3].x));
				test.SetUniformMat4f("model", sun._world.Model * omodel);
				test.SetUniformMat4f("projection", projection);
				test.SetUniformMat4f("view", view);
				glDrawArrays(GL_LINE_LOOP, 0, orbits.size() / 3);
				test.Unbind();
				VAO.Unbind();

				for (unsigned int i = 0; i < planet->numOfChildren; i++)
				{
					VAO.Bind();
					test.Bind();
					glm::mat4 omodel = glm::mat4(1.0f);
					float pos = planet->children[i]->_local.Model[3].x;
					omodel = glm::scale(omodel, glm::vec3(pos, 0.0f, pos));
					test.SetUniformMat4f("model", planet->_world.Model* omodel);
					test.SetUniformMat4f("projection", projection);
					test.SetUniformMat4f("view", view);
					glDrawArrays(GL_LINE_LOOP, 0, orbits.size() / 3);
					test.Unbind();
					VAO.Unbind();
				}
				
			}

			if (isWireFrame)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			{
				ImGui::Begin("Hello, world!");                

				ImGui::Checkbox("Wireframe", &isWireFrame);
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
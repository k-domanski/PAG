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
#include "Shader.h"
#include "Model.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Renderer.h"
#include "Model.h"
#include "Camera.h"
#include "MeshTest.h"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera gCamera(glm::vec3(0.0f, 0.0f, 3.0f));
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

void Menger(int n, const glm::vec3& position, const glm::vec3& scale, drawingData& data);
void Rotate(glm::vec3& current, const glm::vec3& rotation, glm::mat4& view);
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

		//std::vector<float> vertices = {
		//	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,	//0
		//	 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,	//1
		//	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,	//2
		//	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,	//3
		//
		//	-0.5f, -0.5f, 0.5f,  0.0f, 0.0f,		//4
		//	 0.5f, -0.5f, 0.5f,  1.0f, 0.0f,		//5
		//	 0.5f,  0.5f, 0.5f,  1.0f, 1.0f,		//6
		//	-0.5f,  0.5f, 0.5f,  0.0f, 1.0f,		//7
		//
		//	 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,	//8
		//	 0.5f,  0.5f, -0.5f, 0.0f, 1.0f,	//9
		//	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,	//10
		//	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,	//11
		//
		//	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,	//0
		//	-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,	//7
		//	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,	//3
		//	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,	//4
		//
		//	-0.5f,  0.5f, -0.5f, 0.0f, 0.0f,	//3
		//	 0.5f,  0.5f, -0.5f, 1.0f, 0.0f,	//2
		//	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,	//6
		//	-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,	//7
		//
		//	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		//     0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		//	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
		//	-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
		//};

		std::vector<Vertex> vertices =
		{
			Vertex({glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)}),
			Vertex({glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f)}),
			Vertex({glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f)}),
			Vertex({glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f)}),
		
			Vertex({glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)}),
			Vertex({glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)}),
			Vertex({glm::vec3(0.5f,  0.5f, 0.5f), glm::vec2(1.0f, 1.0f)}),
			Vertex({glm::vec3(-0.5f,  0.5f, 0.5f), glm::vec2(0.0f, 1.0f)}),
		
			Vertex({glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)}),
			Vertex({glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f)}),
			Vertex({glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f)}),
			Vertex({glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f)}),
		
			Vertex({glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f)}),
			Vertex({glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f)}),
			Vertex({glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f)}),
			Vertex({glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f)}),
		
			Vertex({glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 0.0f)}),
			Vertex({glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 0.0f)}),
			Vertex({glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f)}),
			Vertex({glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f)}),
		
			Vertex({glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)}),
			Vertex({glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f)}),
			Vertex({glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 1.0f)}),
			Vertex({glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 1.0f)}),
		
		};

		std::vector<unsigned int> indecies = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 10, 9,
			10, 11, 9,

			12,15,14,
			14,13,15,

			16,17,18,
			18,19,16,

			20,21,22,
			22,23,20
		};
		
		
		//VertexArray vao;
		
		//VertexBufferLayout layout;
		//layout.Push<float>(3);
		//layout.Push<float>(2);
		
		//VertexBufferLayout layout;
		//layout.Push<Vertex>(3);
		//layout.Push<Vertex>(2);

		//VertexBuffer vbo(vertices);
		//VertexBuffer vbo(vertices, 5 * 24 * sizeof(float));
		//vao.AddBuffer(vbo, layout);
		
		//IndexBuffer ibo(indecies, 36);

		Shader shader("res/shaders/Basic.shader");

		std::vector<Texture> textures;
		Texture texture("res/textures/hollow-knight.png", "texture_diff");
		textures.push_back(texture);
		Mesh mesh(vertices, indecies, textures);
		texture.Bind(0);
		shader.setUniform1i("u_Texture", 0);
		shader.Unbind();
		//mesh.Draw(shader);
		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 150");

		ImGui::StyleColorsDark();
		
		ImVec4 clear_color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
		ImVec4 texture_color = ImVec4(1.0f, 0.3f, 0.2f, 1.0f);

		glm::mat4 model = glm::mat4(1.0f);

		glm::mat4 mvp = glm::mat4(1.0f);

		bool isWireFrame = false;
		int depth = 0;
		glm::vec3 rot = glm::vec3(0.0f);
		glm::vec3 currentRot = glm::vec3(0.0f);

		drawingData data;

		glm::mat4 camera = glm::mat4(1.0f);
		camera = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			float currentFrame = glfwGetTime();
			gDeltaTime = currentFrame - gLastFrame;
			gLastFrame = currentFrame;

			ProcessInput(window);
			/* Render here */

			renderer.Clear();
			glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = gCamera.GetViewMatrix();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			
			Rotate(currentRot, rot, camera);
			
			{
				Menger(depth, glm::vec3(0.0f), glm::vec3(1.0f), data);
				shader.Bind();
				shader.SetUniform4f("u_Color", texture_color.x, texture_color.y, texture_color.z, texture_color.w);

				if (isWireFrame)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				for (int i = 0; i < data.newPositions.size(); i++)
				{
					model = glm::mat4(1.0f);
					model = glm::translate(model, data.newPositions[i]);
					model = glm::scale(model, data.newScales[i]);
					glm::mat4 mvp = glm::mat4(1.0f);
					mvp = projection * view * model;
					shader.SetUniformMat4f("u_MVP", mvp);
				
					//renderer.Draw(vao, ibo, shader);
					renderer.Draw(mesh.VAO, mesh.GetIBO(), shader);
				}
				data.clearData();
				//glm::mat4 mvp = projection * view * model;
				//shader.SetUniformMat4f("u_MVP", mvp);
				//renderer.Draw(mesh.VAO, mesh.GetIBO(), shader);
			}

			{
				ImGui::Begin("Hello, world!");                

				ImGui::Checkbox("Wireframe", &isWireFrame);
				ImGui::SliderFloat3("rotation", (float*)& rot, -360.0f, 360.0f);
				ImGui::SliderInt("Depth", &depth, 0, 3);
				ImGui::ColorEdit3("clear color", (float*)& clear_color);
				ImGui::ColorEdit3("texture color", (float*)& texture_color);

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

void Menger(int n, const glm::vec3& position, const glm::vec3& scale, drawingData& data)
{
	glm::vec3 newPosition;
	glm::vec3 newScale;

	if (n == 0)
	{
		data.newPositions.push_back(position);
		data.newScales.push_back(scale);
	}
	else
	{
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				for (int k = -1; k < 2; k++)
				{
					if ((i * i + j * j) * (i * i + k * k) * (j * j + k * k) > 0)
					{
						newScale = scale * (1.0f / 3.0f);
						newPosition = glm::vec3(position.x + i * newScale.x,
							position.y + j * newScale.y,
							position.z + k * newScale.z);

						Menger(n - 1, newPosition, newScale, data);
					}
				}
			}
		}
	}
}

void Rotate(glm::vec3 & current, const glm::vec3 & rotation, glm::mat4 & view)
{
	glm::vec3 change = glm::vec3(0.0f);
	change = rotation - current;
	glm::vec3 vec[] =
	{
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f)
	};

	for (int i = 0; i < 3; i++)
	{
		current[i] != rotation[i] ? view = glm::rotate_slow(view, glm::radians(change[i]), vec[i]) : view = glm::rotate(view, 0.0f, vec[i]);
	}

	current = rotation;
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
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

const unsigned int N_DIR = 1;
const unsigned int N_POINT = 2;
const unsigned int N_SPOT = 3;


Camera gCamera(glm::vec3(0.0f, 30.0f, 30.0f));
float gLastX = SCR_WIDTH / 2.0f;
float gLastY = SCR_HEIGHT / 2.0f;
bool gFirstMouse = true;

float gDeltaTime = 0.0f;
float gLastFrame = 0.0f;

struct DirLight
{
	glm::vec3 direction = glm::vec3(-1.0f,-1.0f, 0.0f);

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	bool isActive = true;
	float intensity = 1.0f;

	DirLight()
	{
		ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		specular = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	void SetUniforms(Shader& shader, const std::string& lightName)
	{
		shader.SetUniformVec3f(lightName + ".direction", direction);

		shader.SetUniformVec3f(lightName + ".ambient", ambient);
		shader.SetUniformVec3f(lightName + ".diffuse", diffuse);
		shader.SetUniformVec3f(lightName + ".specular", specular);

		shader.setUniform1i(lightName + ".isActive", isActive);
		shader.SetUniform1f(lightName + ".intensity", intensity);
	}
};

struct PointLight
{
	glm::vec3 position;

	float constant;
	float _linear;
	float quadratic;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	bool isActive = true;

	PointLight()
	{
		constant = 1.0f;
		_linear = 0.007f;//0.0014f;//0.09f;
		quadratic = 0.0002f;//0.000007f;//0.032f;

		ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		specular = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	void SetUniforms(Shader& shader, const std::string& lightName)
	{
		shader.SetUniformVec3f(lightName + ".position", position);

		shader.SetUniform1f(lightName + ".constant", constant);
		shader.SetUniform1f(lightName + "._linear", _linear);
		shader.SetUniform1f(lightName + ".quadratic", quadratic);

		shader.SetUniformVec3f(lightName + ".ambient", ambient);
		shader.SetUniformVec3f(lightName + ".diffuse", diffuse);
		shader.SetUniformVec3f(lightName + ".specular", specular);

		shader.setUniform1i(lightName + ".isActive", isActive);
	}
};

struct SpotLight
{
	glm::vec3 position;
	glm::vec3 direction;

	float cutOff;
	float outerCutOff;

	float constant;
	float _linear;
	float quadratic;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	bool isActive = true;

	SpotLight()
	{
		cutOff = glm::cos(glm::radians(12.5f));
		outerCutOff = glm::cos(glm::radians(17.5f));

		constant = 1.0f;
		_linear = 0.09f;
		quadratic = 0.032f;

		ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		specular = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	void SetUniforms(Shader& shader, const std::string& lightName)
	{
		//shader.SetUniformVec3f(lightName + ".position", position);
		//shader.SetUniformVec3f(lightName + ".direction", direction);

		shader.SetUniform1f(lightName + ".cutOff", cutOff);
		shader.SetUniform1f(lightName + ".outerCutOff", outerCutOff);
		shader.SetUniform1f(lightName + ".constant", constant);
		shader.SetUniform1f(lightName + "._linear", _linear);
		shader.SetUniform1f(lightName + ".quadratic", quadratic);

		shader.SetUniformVec3f(lightName + ".ambient", ambient);
		shader.SetUniformVec3f(lightName + ".diffuse", diffuse);
		shader.SetUniformVec3f(lightName + ".specular", specular);

		shader.setUniform1i(lightName + ".isActive", isActive);
	}
};

struct PBRMaterial
{
	glm::vec3 albedo;
	float metallic;
	float roughness;
	float ao;

	PBRMaterial()
	{
		albedo = glm::vec3(0.5f, 0.0f, 0.0f);
		metallic = 0.5f;
		roughness = 0.5f;
		ao = 1.0f;
	}

	void SetUniforms(Shader& shader, const std::string& materialName)
	{

		shader.SetUniformVec3f(materialName + ".albedo", albedo);
		shader.SetUniform1f(materialName + ".metallic", metallic);
		shader.SetUniform1f(materialName + ".roughness", roughness);
		shader.SetUniform1f(materialName + ".ao", ao);
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
		Shader PBR("res/shaders/PBR.shader", 2);

		Texture roofTexture("roof.png", "res/textures", "diffuse");
		roofTexture.Bind(0);
		Texture houseTexture("brickWall.jpg", "res/textures", "diffuse");
		houseTexture.Bind(0);
		Texture groundTexture("grass1.jpg", "res/textures", "diffuse");
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
		//root.World().Model = glm::rotate(root.World().Model, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

		for (int i = -10; i < 11; i++)
		{
			for (int j = -10; j < 11; j++)
			{
				SceneNode test(glm::vec3((float)i, 0.0f, (float)j), glm::vec3(5.0f));
				SceneNode test1(glm::vec3(0.0f, 0.1f, 0.05f), glm::vec3(1.0f));
				test.AddChild(test1);
				root.AddChild(test);
			}
		}
		
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
		SceneNode dl1(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f));
		root.AddChild(dl1);
		SceneNode pl1(glm::vec3(0.5f, 1.5f, 5.0f), glm::vec3(4.0f));
		root.AddChild(pl1);
		SceneNode pl2(glm::vec3(0.5f, 2.0f, 0.0f), glm::vec3(4.0f));
		root.AddChild(pl2);
		SceneNode sl1(glm::vec3(0.5f, 125.0f, 0.0f), glm::vec3(1.0f));
		root.AddChild(sl1);
		SceneNode sl2(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(1.0f));
		root.AddChild(sl2);
		SceneNode sl3(glm::vec3(5.f, 5.0f, 5.0f), glm::vec3(1.0f));
		root.AddChild(sl3);
		SceneNode ground(glm::vec3(0.0f, -0.2f, 0.0f), glm::vec3(2500.0f,1.0f, 2500.0f));
		root.AddChild(ground);
		root.calculateWorld(root, root.World());
		data2.push_back(root.Children()[root.Children().size() - 1].World().Model);
		VertexBuffer roofBuffer(data1);
		VertexBuffer houseBuffer(data);
		VertexBuffer groundBuffer(data2);
		
		
		PointLight pointLight;
		PointLight pointLight1;
		pointLight.position = root.Children()[root.Children().size() - 5].World().Model[3];
		pointLight1.position = root.Children()[root.Children().size() - 6].World().Model[3];
		
		PointLight* pointLights[2];
		pointLights[0] = &pointLight;
		pointLights[1] = &pointLight1;
		
		DirLight directional;
		SpotLight spotLight1;
		SpotLight spotLight2;
		SpotLight spotLight3;
		spotLight2.position = glm::vec3(0.0f, 5.0f, 0.0f);
		spotLight2.direction = glm::vec3(1.0f, -1.0f, 0.0f);

		spotLight3.position = glm::vec3(5.0f, 5.0f, 5.0f);
		spotLight3.direction = glm::vec3(0.0f, -1.0f, 0.0f);
		SpotLight* spotLights[N_SPOT];
		spotLights[0] = &spotLight1;
		spotLights[1] = &spotLight2;
		spotLights[2] = &spotLight3;
		
		PBRMaterial pbrMaterial;

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

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 150");

		ImGui::StyleColorsDark();
		
		ImVec4 clear_color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
		glm::vec3 rotDir = glm::vec3(0.0f, 0.0f, 1.0f);
		bool isWireFrame = false;
		bool showdemowindow = false;
		bool isPBR = false;
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
			glm::mat4 test = glm::rotate(glm::mat4(1.0f), 0.01f, rotDir);
			root.Children()[root.Children().size() - 5].World().Model = test * root.Children()[root.Children().size() - 5].World().Model;
			pointLights[0]->position = root.Children()[root.Children().size() - 5].World().Model[3];
			
			if (isPBR)
			{
				for (int i = 0; i < N_SPOT; i++)
				{
					spotLights[i]->isActive = false;
				}
				//directional.isActive = false;
				
				PBR.Bind();
				PBR.SetUniformMat4f("projection", projection);
				PBR.SetUniformMat4f("view", view);

				pbrMaterial.SetUniforms(PBR, "material");
				PBR.SetUniformVec3f("viewPos", gCamera.Position);

				for (int i = 0; i < N_POINT; i++)
				{
					pointLights[i]->SetUniforms(PBR, "pointLight[" + std::to_string(i) + "]");
				}

				for (int i = 0; i < N_DIR; i++)
					directional.SetUniforms(shader, "dirLight[" + std::to_string(i) + "]");
				/*roof Draw*/
				roofVAO.Bind();
				roofTexture.Bind(0);
				glDrawArraysInstanced(GL_TRIANGLES, 0, 36, data1.size());
				roofVAO.Unbind();
				roofTexture.Unbind();

				/*house Draw*/
				houseVAO.Bind();
				houseTexture.Bind(0);
				glDrawArraysInstanced(GL_TRIANGLES, 0, 36, data.size());
				houseVAO.Unbind();
				houseTexture.Unbind();

				/*ground Draw*/
				groundVao.Bind();
				groundTexture.Bind(0);
				Renderer::Draw(groundVao, groundIbo, PBR);
				groundTexture.Unbind();

				PBR.Unbind();
			}
			else
			{
				shader.Bind();
				shader.SetUniformMat4f("projection", projection);
				shader.SetUniformMat4f("view", view);

				shader.SetUniformVec3f("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
				shader.SetUniformVec3f("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
				shader.SetUniformVec3f("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));

				shader.SetUniform1f("material.shininess", 32.0f);

				shader.SetUniformVec3f("spotLight[0].position", gCamera.Position);
				shader.SetUniformVec3f("spotLight[0].direction", gCamera.Front);

				shader.SetUniformVec3f("spotLight[1].position", spotLight2.position);
				shader.SetUniformVec3f("spotLight[1].direction", spotLight2.direction);
				shader.SetUniformVec3f("spotLight[2].position", spotLight3.position);
				shader.SetUniformVec3f("spotLight[2].direction", spotLight3.direction);

				shader.SetUniformVec3f("viewPos", gCamera.Position);
				

				for (int i = 0; i < N_POINT; i++)
				{
					pointLights[i]->SetUniforms(shader, "pointLight[" + std::to_string(i) + "]");
				}

				for (int i = 0; i < N_DIR; i++)
					directional.SetUniforms(shader, "dirLight[" + std::to_string(i) + "]");

				for (int i = 0; i < N_SPOT; i++)
					spotLights[i]->SetUniforms(shader, "spotLight[" + std::to_string(i) + "]");

				/*roof Draw*/
				roofVAO.Bind();
				roofTexture.Bind(0);
				glDrawArraysInstanced(GL_TRIANGLES, 0, 36, data1.size());
				roofVAO.Unbind();
				roofTexture.Unbind();

				/*house Draw*/
				houseVAO.Bind();
				houseTexture.Bind(0);
				glDrawArraysInstanced(GL_TRIANGLES, 0, 36, data.size());
				houseVAO.Unbind();
				houseTexture.Unbind();

				/*ground Draw*/
				groundVao.Bind();
				groundTexture.Bind(0);
				Renderer::Draw(groundVao, groundIbo, shader);
				groundTexture.Unbind();

				shader.Unbind();
			}
			
			
			
			

			/*light container draw*/
			light.Bind();
			light.SetUniformMat4f("projection", projection);
			light.SetUniformMat4f("view", view);
			
			
			light.SetUniformMat4f("model", root.Children()[root.Children().size()  - 6].World().Model);
			if(pointLight1.isActive)
				Renderer::Draw(lightVAO, light);
			light.SetUniformMat4f("model", root.Children()[root.Children().size()  - 5].World().Model);
			if(pointLight.isActive)
				Renderer::Draw(lightVAO, light);
			
			for (int i = 0; i < N_SPOT; i++)
			{
				glm::mat4 m = glm::mat4(1.0f);
				m = glm::translate(m, spotLights[i]->position);
				light.SetUniformMat4f("model", m);
				if (spotLights[i]->isActive)
					Renderer::Draw(lightVAO, light);
			}
			{
				glm::mat4 s = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
				glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(directional.direction.x, -directional.direction.y,directional.direction.z) * 10.0f);
				light.SetUniformMat4f("model", m * s);
				if(directional.isActive)
					Renderer::Draw(lightVAO, light);
			}
			light.Unbind();
			lightVAO.Unbind();

			if (isWireFrame)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			{
				ImGui::Begin("Hello, world!");
				ImGui::Checkbox("Wireframe", &isWireFrame);
				ImGui::Checkbox("PBR", &isPBR);
				//ImGui::Checkbox("Demo", &showdemowindow);
				//if (showdemowindow)
				//	ImGui::ShowDemoWindow();
				
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				if (ImGui::CollapsingHeader("Lights"))
				{
					if (isPBR)
					{
						if (ImGui::TreeNode("material"))
						{
							ImGui::SliderFloat("metallic", (float*)& pbrMaterial.metallic, 0.0f, 1.0f);
							ImGui::SliderFloat("roughness", (float*)& pbrMaterial.roughness, 0.0f, 1.0f);
							ImGui::SliderFloat("ao", (float*)& pbrMaterial.ao, 0.0f, 1.0f);
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("point1"))
						{
							ImGui::Checkbox("on/off", &pointLight.isActive);
							ImGui::ColorEdit3("position", (float*)& pointLight.position);
							ImGui::ColorEdit3("diffuse", (float*)& pointLight.diffuse);
							ImGui::TreePop();
						}

						if (ImGui::TreeNode("point2"))
						{
							ImGui::Checkbox("on/off", &pointLight1.isActive);
							ImGui::ColorEdit3("position", (float*)& pointLight1.position);
							ImGui::ColorEdit3("diffuse", (float*)& pointLight1.diffuse);
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("directional"))
						{
							ImGui::Checkbox("on/off", &directional.isActive);
							ImGui::SliderFloat3("direction", (float*)& directional.direction, -1.0f, 1.0f);
							ImGui::ColorEdit3("diffuse", (float*)& directional.diffuse);
							ImGui::TreePop();
						}
					}
					else
					{
						if (ImGui::TreeNode("point1"))
						{
							ImGui::Checkbox("on/off", &pointLight.isActive);
							ImGui::SliderFloat3("position", (float*)& pointLight.position, -100.0f, 100.0f);
							ImGui::SliderFloat3("direction", (float*)& rotDir, -1.0f, 1.0f);
							ImGui::ColorEdit3("ambient", (float*)& pointLight.ambient);
							ImGui::ColorEdit3("diffuse", (float*)& pointLight.diffuse);
							ImGui::ColorEdit3("specular", (float*)& pointLight.specular);
							ImGui::TreePop();
						}

						if (ImGui::TreeNode("point2"))
						{
							ImGui::Checkbox("on/off", &pointLight1.isActive);
							ImGui::ColorEdit3("ambient", (float*)& pointLight1.ambient);
							ImGui::ColorEdit3("diffuse", (float*)& pointLight1.diffuse);
							ImGui::ColorEdit3("specular", (float*)& pointLight1.specular);
							ImGui::TreePop();
						}

						if (ImGui::TreeNode("directional"))
						{
							ImGui::Checkbox("on/off", &directional.isActive);
							ImGui::SliderFloat3("direction", (float*)& directional.direction, -1.0f, 1.0f);
							ImGui::SliderFloat("intensity", (float*)& directional.intensity, 0.0f, 1.0f);
							ImGui::ColorEdit3("ambient", (float*)& directional.ambient);
							ImGui::ColorEdit3("diffuse", (float*)& directional.diffuse);
							ImGui::ColorEdit3("specular", (float*)& directional.specular);
							ImGui::TreePop();
						}

						if (ImGui::TreeNode("spotlight1"))
						{
							ImGui::Checkbox("on/off", &spotLight1.isActive);
							//ImGui::SliderFloat3("direction", (float*)& spotLight1.direction, -1.0f, 1.0f);
							//ImGui::SliderFloat3("position", (float*)& spotLight1.position, -10.0f, 10.0f);
							ImGui::ColorEdit3("ambient", (float*)& spotLight1.ambient);
							ImGui::ColorEdit3("diffuse", (float*)& spotLight1.diffuse);
							ImGui::ColorEdit3("specular", (float*)& spotLight1.specular);
							ImGui::TreePop();
						}

						if (ImGui::TreeNode("spotlight2"))
						{
							ImGui::Checkbox("on/off", &spotLight2.isActive);
							ImGui::SliderFloat3("direction", (float*)& spotLight2.direction, -1.0f, 1.0f);
							ImGui::SliderFloat3("position", (float*)& spotLight2.position, -10.0f, 10.0f);
							ImGui::ColorEdit3("ambient", (float*)& spotLight2.ambient);
							ImGui::ColorEdit3("diffuse", (float*)& spotLight2.diffuse);
							ImGui::ColorEdit3("specular", (float*)& spotLight2.specular);
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("spotlight3"))
						{
							ImGui::Checkbox("on/off", &spotLight3.isActive);
							ImGui::SliderFloat3("direction", (float*)& spotLight3.direction, -1.0f, 1.0f);
							ImGui::SliderFloat3("position", (float*)& spotLight3.position, -10.0f, 10.0f);
							ImGui::ColorEdit3("ambient", (float*)& spotLight3.ambient);
							ImGui::ColorEdit3("diffuse", (float*)& spotLight3.diffuse);
							ImGui::ColorEdit3("specular", (float*)& spotLight3.specular);
							ImGui::TreePop();
						}
					}
					
					
				}
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
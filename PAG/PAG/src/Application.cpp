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
#include "SkyBox.h"
#include "Player.h"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


const unsigned int SHADOW_WIDTH= 1024;
const unsigned int SHADOW_HEIGHT = 1024;

const unsigned int N_DIR = 1;
const unsigned int N_POINT = 2;
const unsigned int N_SPOT = 3;


Camera gCamera(glm::vec3(0.0f, 0.0f, 30.0f));
Player* gPlayer = new Player();
float gLastX = SCR_WIDTH / 2.0f;
float gLastY = SCR_HEIGHT / 2.0f;
bool gFirstMouse = true;
bool isCamera = true;

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

		std::vector<unsigned int> groundInd =
		{
			31,33,34,
			34, 31, 35
		};
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
		std::vector<float> skyboxVertices =
		{
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
		/*skybox*/
		VertexArray skyboxVao;
		VertexBuffer skyboxVbo(skyboxVertices, skyboxVertices.size() * sizeof(float));
		VertexBufferLayout skyboxLayout;
		skyboxLayout.Push<float>(3);
		skyboxVao.AddBuffer(skyboxVbo, skyboxLayout);
		std::vector<std::string> faces =
		{
			"res/textures/skybox/right.jpg",
			"res/textures/skybox/left.jpg",
			"res/textures/skybox/top.jpg",
			"res/textures/skybox/bottom.jpg",
			"res/textures/skybox/front.jpg",
			"res/textures/skybox/back.jpg"
		};
		Skybox skybox(faces, skyboxVao);
		Shader skyboxShader("res/shaders/skybox.shader", 3);

		Shader shader("res/shaders/noLight.shader", 0);
		Shader lightShader("res/shaders/Shadows.shader", 1);
		Shader reflectShader("res/shaders/reflection.shader", 4);
		Shader refractShader("res/shaders/refraction.shader", 5);
		Shader depthShader("res/shaders/DepthShader.shader", 6);

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
		


		/*DepthMAP*/
		unsigned int depthMapFBO;
		glGenFramebuffers(1, &depthMapFBO);

		unsigned int depthMap;
		glGenTextures(1, &depthMap);

		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);





		//Model backpack("res/models/backpack/backpack.obj");
		//Model nanosuit("res/models/nanosuit/nanosuit.obj");
		Model nanosuit("res/models/helikopter2.obj");
		Model smiglo("res/models/smiglo1.obj");
		Model nogi("res/models/nogi.obj");
		SceneNode* root =  new SceneNode(glm::vec3(0.0f), glm::vec3(1.0f));
		SceneNode* box1 =  new SceneNode(glm::vec3(0.0f), glm::vec3(1.0f));
		SceneNode* smig=  new SceneNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
		SceneNode* nog =  new SceneNode(glm::vec3(0.0f), glm::vec3(1.0f));
		SceneNode* box2 =  new SceneNode(glm::vec3(0.0f), glm::vec3(1000.0f, 0.0f, 1000.0f));
		//SceneNode* camera = new SceneNode(glm::vec3(-60.0f, 10.0f, 0.0f), glm::vec3(1.0f));
		SceneNode* camera = new SceneNode(glm::vec3(0.0f, 10.0f, -30.0f), glm::vec3(1.0f));
		SceneNode* houseRoot = new SceneNode(glm::vec3(0.0f), glm::vec3(100.0f));
		SceneNode* house1 = new SceneNode(glm::vec3(0.0f, 0.05f, 0.0f), glm::vec3(1.0f));
		SceneNode* roof1 = new SceneNode(glm::vec3(0.0f, 0.1f, 0.05f), glm::vec3(1.0f));
		root->AddChild(box1);
		root->AddChild(box2);
		root->AddChild(houseRoot);
		houseRoot->AddChild(house1);
		house1->AddChild(roof1);
		box1->AddChild(camera);
		box1->AddChild(smig);
		box1->AddChild(nog);
		
		gPlayer->model = box1;
		
		/*LIGHTS*/
		SceneNode* dl1 = new SceneNode(glm::vec3(-2.0f, 10.0f, -1.0f), glm::vec3(1.0f));
		//SceneNode* dl1 = new SceneNode(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f));
		root->AddChild(dl1);
		
		
		DirLight directional;
		//houseVAO.Bind();
		//houseBuffer.Bind();
		//glEnableVertexAttribArray(3);
		//glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		//glEnableVertexAttribArray(4);
		//glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		//glEnableVertexAttribArray(5);
		//glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		//glEnableVertexAttribArray(6);
		//glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		//houseBuffer.Unbind();
		//glVertexAttribDivisor(3, 1);
		//glVertexAttribDivisor(4, 1);
		//glVertexAttribDivisor(5, 1);
		//glVertexAttribDivisor(6, 1);
		//houseVAO.Unbind();

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
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			glm::mat4 lightProjection, lightView;
			glm::mat4 lightSpaceMatrix;
			float near_plane = 1.0f, far_plane = 7.5f;
			lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
			glm::vec3 lightPos = dl1->World()[3];
			lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
			lightSpaceMatrix = lightProjection * lightView;
			// render scene from light's point of view
			depthShader.Bind();
			depthShader.SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);

			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			

			depthShader.SetUniformMat4f("model", house1->World());
			houseVAO.Bind();
			houseTexture.Bind();
			glActiveTexture(GL_TEXTURE1);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			houseVAO.Unbind();
			houseTexture.Unbind();
			roofVAO.Bind();
			roofTexture.Bind();
			depthShader.SetUniformMat4f("model", roof1->World());
			glDrawArrays(GL_TRIANGLES, 0, 36);
			roofVAO.Unbind();
			roofTexture.Unbind();
			depthShader.SetUniformMat4f("model", box2->World());
			groundVao.Bind();
			groundTexture.Bind();
			Renderer::Draw(groundVao, groundIbo, depthShader);
			groundVao.Unbind();
			groundTexture.Unbind();
			depthShader.Unbind();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			
			

			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//test = glm::rotate(glm::mat4(1.0f), gDeltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
			smig->Local() = glm::rotate(smig->Local(), gDeltaTime * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			//smig->Local() = test * smig->Local();
			Renderer::Clear();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			//player->model->Local() = glm::rotate(player->model->Local(), gDeltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
			root->calculateWorld(root->World());
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
			glm::mat4 view;
			if (isCamera)
			{
				//gCamera.Position = gPlayer->model->World()[3] + glm::vec4(0.0f,1.0f,3.0f,0.0f);
				gCamera.Position = camera->World()[3];
				//gCamera.Front = glm::normalize(gPlayer->model->Local()[3]);
				//gCamera.Front = -gPlayer->getFront();
				glm::vec3 front = gPlayer->model->World()[3];
				glm::vec3 pos = camera->World()[3];
				view = glm::lookAt(gCamera.Position, front + glm::vec3(0.0f, 1.0f, 0.0f), gCamera.Up);
			}
			else
			{
				
				view = gCamera.GetViewMatrix();
			}
			//glm::mat4 view = gCamera.GetViewMatrix();
			
			
			//shader.Bind();
			//shader.SetUniformMat4f("projection", projection);
			//shader.SetUniformMat4f("view", view);
			//
			//shader.SetUniformVec3f("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
			//shader.SetUniformVec3f("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
			//shader.SetUniformVec3f("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			//
			//shader.SetUniform1f("material.shininess", 32.0f);
			//
			//shader.SetUniformVec3f("spotLight[0].position", gCamera.Position);
			//shader.SetUniformVec3f("spotLight[0].direction", gCamera.Front);
			//
			//shader.SetUniformVec3f("spotLight[1].position", spotLight2.position);
			//shader.SetUniformVec3f("spotLight[1].direction", spotLight2.direction);
			//shader.SetUniformVec3f("spotLight[2].position", spotLight3.position);
			//shader.SetUniformVec3f("spotLight[2].direction", spotLight3.direction);
			//
			//shader.SetUniformVec3f("viewPos", gCamera.Position);
			//
			//
			//for (int i = 0; i < N_POINT; i++)
			//{
			//	pointLights[i]->SetUniforms(shader, "pointLight[" + std::to_string(i) + "]");
			//}
			//
			//for (int i = 0; i < N_DIR; i++)
			//	directional.SetUniforms(shader, "dirLight[" + std::to_string(i) + "]");
			//
			//for (int i = 0; i < N_SPOT; i++)
			//	spotLights[i]->SetUniforms(shader, "spotLight[" + std::to_string(i) + "]");

			

			/*house Draw*/
			//houseVAO.Bind();
			//houseTexture.Bind(0);
			//glDrawArraysInstanced(GL_TRIANGLES, 0, 36, data.size());
			//houseVAO.Unbind();
			//houseTexture.Unbind();
			//
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			lightShader.Bind();
			lightShader.SetUniformMat4f("view", view);
			lightShader.SetUniformMat4f("projection", projection);
			lightShader.SetUniformMat4f("model",house1->World());
			lightShader.SetUniformVec3f("viewPos", gCamera.Position);
			lightShader.SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
			//lightShader.SetUniformVec3f("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
			//lightShader.SetUniformVec3f("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
			lightShader.SetUniformVec3f("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			lightShader.SetUniform1f("material.shininess", 32.0f);
			directional.SetUniforms(lightShader, "dirLight[0]");
			houseVAO.Bind();
			houseTexture.Bind();
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			houseVAO.Unbind();
			houseTexture.Unbind();
			roofVAO.Bind();
			roofTexture.Bind();
			lightShader.SetUniformMat4f("model", roof1->World());
			glDrawArrays(GL_TRIANGLES, 0, 36);
			roofVAO.Unbind();
			roofTexture.Unbind();
			lightShader.SetUniformMat4f("model", box2->World());
			groundVao.Bind();
			groundTexture.Bind();
			Renderer::Draw(groundVao,groundIbo, lightShader);
			groundVao.Unbind();
			groundTexture.Unbind();
			lightShader.Unbind();
			/*ground Draw*/
			//shader.Bind();
			//shader.SetUniformMat4f("view", view);
			//shader.SetUniformMat4f("projection", projection);


			reflectShader.Bind();
			reflectShader.SetUniformMat4f("view", view);
			reflectShader.SetUniformMat4f("projection", projection);
			reflectShader.SetUniformVec3f("cameraPos", gCamera.Position);
			
			
			
			reflectShader.SetUniformMat4f("model", smig->World());
			glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetTexture());
			smiglo.Draw(reflectShader);

			reflectShader.SetUniformMat4f("model", nog->World());
			glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetTexture());
			//nogi.Draw(reflectShader);

			reflectShader.Unbind();

			shader.Bind();
			
			
			shader.SetUniformMat4f("view", view);
			shader.SetUniformMat4f("projection", projection);
			shader.SetUniformMat4f("model", gPlayer->model->World());
			//glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetTexture());
			//groundTexture.Bind(0);
			nanosuit.Draw(shader);
			shader.Unbind();
			///*roof Draw*/
			//refractShader.Bind();
			//refractShader.SetUniformMat4f("view", view);
			//refractShader.SetUniformMat4f("projection", projection);
			//refractShader.SetUniformVec3f("cameraPos", gCamera.Position);
			////roofVAO.Bind();
			//houseVAO.Bind();
			//glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetTexture());
			////roofTexture.Bind(0);
			//glDrawArraysInstanced(GL_TRIANGLES, 0, 36, data1.size());
			////roofVAO.Unbind();
			//houseVAO.Unbind();
			//refractShader.Unbind();
			////roofTexture.Unbind();

			/*skybox*/
			skyboxShader.Bind();
			glm::mat4 skyboxview = glm::mat4(1.0f);
			skyboxview = glm::mat4(glm::mat3(gCamera.GetViewMatrix()));
			skyboxShader.SetUniformMat4f("view", skyboxview);
			skyboxShader.SetUniformMat4f("projection", projection);
			skybox.Draw();
			

			///*light container draw*/
			//light.Bind();
			//light.SetUniformMat4f("projection", projection);
			//light.SetUniformMat4f("view", view);
			//
			//
			//light.SetUniformMat4f("model", root.Children()[root.Children().size()  - 6].World().Model);
			//if(pointLight1.isActive)
			//	Renderer::Draw(lightVAO, light);
			//light.SetUniformMat4f("model", root.Children()[root.Children().size()  - 5].World().Model);
			//if(pointLight.isActive)
			//	Renderer::Draw(lightVAO, light);
			//
			//for (int i = 0; i < N_SPOT; i++)
			//{
			//	glm::mat4 m = glm::mat4(1.0f);
			//	m = glm::translate(m, spotLights[i]->position);
			//	light.SetUniformMat4f("model", m);
			//	if (spotLights[i]->isActive)
			//		Renderer::Draw(lightVAO, light);
			//}
			//{
			//	glm::mat4 s = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
			//	glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(directional.direction.x, -directional.direction.y,directional.direction.z) * 10.0f);
			//	light.SetUniformMat4f("model", m * s);
			//	if(directional.isActive)
			//		Renderer::Draw(lightVAO, light);
			//}
			//light.Unbind();
			//lightVAO.Unbind();

			if (isWireFrame)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			{
				ImGui::Begin("Hello, world!");
				ImGui::Checkbox("Wireframe", &isWireFrame);
				ImGui::Checkbox("Camera", &isCamera);
				//ImGui::Checkbox("Demo", &showdemowindow);
				//if (showdemowindow)
				//	ImGui::ShowDemoWindow();
				
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				//if (ImGui::CollapsingHeader("Lights"))
				//{
				//		if (ImGui::TreeNode("point1"))
				//		{
				//			ImGui::Checkbox("on/off", &pointLight.isActive);
				//			ImGui::SliderFloat3("position", (float*)& pointLight.position, -100.0f, 100.0f);
				//			ImGui::SliderFloat3("direction", (float*)& rotDir, -1.0f, 1.0f);
				//			ImGui::ColorEdit3("ambient", (float*)& pointLight.ambient);
				//			ImGui::ColorEdit3("diffuse", (float*)& pointLight.diffuse);
				//			ImGui::ColorEdit3("specular", (float*)& pointLight.specular);
				//			ImGui::TreePop();
				//		}
				//
				//		if (ImGui::TreeNode("point2"))
				//		{
				//			ImGui::Checkbox("on/off", &pointLight1.isActive);
				//			ImGui::ColorEdit3("ambient", (float*)& pointLight1.ambient);
				//			ImGui::ColorEdit3("diffuse", (float*)& pointLight1.diffuse);
				//			ImGui::ColorEdit3("specular", (float*)& pointLight1.specular);
				//			ImGui::TreePop();
				//		}
				//
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
				//
				//		if (ImGui::TreeNode("spotlight1"))
				//		{
				//			ImGui::Checkbox("on/off", &spotLight1.isActive);
				//			//ImGui::SliderFloat3("direction", (float*)& spotLight1.direction, -1.0f, 1.0f);
				//			//ImGui::SliderFloat3("position", (float*)& spotLight1.position, -10.0f, 10.0f);
				//			ImGui::ColorEdit3("ambient", (float*)& spotLight1.ambient);
				//			ImGui::ColorEdit3("diffuse", (float*)& spotLight1.diffuse);
				//			ImGui::ColorEdit3("specular", (float*)& spotLight1.specular);
				//			ImGui::TreePop();
				//		}
				//
				//		if (ImGui::TreeNode("spotlight2"))
				//		{
				//			ImGui::Checkbox("on/off", &spotLight2.isActive);
				//			ImGui::SliderFloat3("direction", (float*)& spotLight2.direction, -1.0f, 1.0f);
				//			ImGui::SliderFloat3("position", (float*)& spotLight2.position, -10.0f, 10.0f);
				//			ImGui::ColorEdit3("ambient", (float*)& spotLight2.ambient);
				//			ImGui::ColorEdit3("diffuse", (float*)& spotLight2.diffuse);
				//			ImGui::ColorEdit3("specular", (float*)& spotLight2.specular);
				//			ImGui::TreePop();
				//		}
				//		if (ImGui::TreeNode("spotlight3"))
				//		{
				//			ImGui::Checkbox("on/off", &spotLight3.isActive);
				//			ImGui::SliderFloat3("direction", (float*)& spotLight3.direction, -1.0f, 1.0f);
				//			ImGui::SliderFloat3("position", (float*)& spotLight3.position, -10.0f, 10.0f);
				//			ImGui::ColorEdit3("ambient", (float*)& spotLight3.ambient);
				//			ImGui::ColorEdit3("diffuse", (float*)& spotLight3.diffuse);
				//			ImGui::ColorEdit3("specular", (float*)& spotLight3.specular);
				//			ImGui::TreePop();
				//		}
				//	}
				//	
				//	
				//}
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
	if (!isCamera)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			gCamera.ProcessKeyboard(LEFT, gDeltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
	}
	else
	{
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			gPlayer->PlayerInput(U, gDeltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		{
			gPlayer->PlayerInput(F, gDeltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
			gPlayer->PlayerInput(D, gDeltaTime);
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
			gPlayer->PlayerInput(L, gDeltaTime);
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			gPlayer->PlayerInput(R, gDeltaTime);
	}
	
	
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

	if (isCamera)
		gPlayer->calculateMouseInput(xoffset,yoffset, gDeltaTime);
	else
	{
		gCamera.ProcessMouseMovement(xoffset, yoffset);
	}
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}
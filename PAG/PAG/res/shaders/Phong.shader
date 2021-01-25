#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoord;

out vec3 fNormal;
out vec3 fPos;
out vec2 fTextCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fPos = vec3(model * vec4(position, 1.0));
	gl_Position = projection * view * vec4(fPos, 1.0);
	//gl_Position = projection * view * aInstanceModel * vec4(position, 1.0);
	fNormal = mat3(transpose(inverse(model))) * aNormal;

	fTextCoord = aTextCoord;
};

#shader fragment
#version 330 core
out vec4 FragColor;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct PointLight
{
	vec3 position;

	float constant;
	float _linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	bool isActive;
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	bool isActive;
	float intensity;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;

	float cutOff;
	float outerCutOff;
	float constant;
	float _linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	bool isActive;
};

in vec3 fNormal;
in vec3 fPos;
in vec2 fTextCoord;

#define NUM_DIR_LIGHT 1

uniform Material material;
uniform DirLight dirLight[NUM_DIR_LIGHT];
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D Texture;


vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
	vec3 viewDir = normalize(viewPos - fPos);

	vec3 result = vec3(0.0);

	for (int i = 0; i < NUM_DIR_LIGHT; i++)
	{
		if (dirLight[i].isActive)
			result += calculateDirLight(dirLight[i], fNormal, viewDir);
	}

	FragColor = vec4(result, 1.0);
};

vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	/*ambient*/
	vec3 ambient = light.ambient * texture(Texture, fTextCoord).rgb;

	/*diffuse*/
	vec3 norm = normalize(normal);
	//vec3 lightDir = normalize(lightPos - fPos);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(Texture, fTextCoord).rgb;

	/*specular*/
	//vec3 viewDir = normalize(viewPos - fPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);


	vec3 result = (ambient + diffuse + specular) * light.intensity;
	return result;
}



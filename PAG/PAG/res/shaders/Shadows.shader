#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out VS_OUT
{
	vec3 fNormal;
	vec3 fPos;
	vec2 fTexCoord;
	vec4 FragPosLightSpace;
} vs_out;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
	vs_out.fPos = vec3(model * vec4(aPos, 1.0));
	vs_out.fNormal = transpose(inverse(mat3(model))) * aNormal;
	vs_out.fTexCoord = aTexCoord;
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.fPos, 1.0);
	gl_Position = projection * view * vec4(vs_out.fPos, 1.0);



	//fPos = vec3(model * vec4(aPos, 1.0));
	//gl_Position = projection * view * vec4(fPos, 1.0);
	////gl_Position = projection * view * aInstanceModel * vec4(position, 1.0);
	//fNormal = mat3(transpose(inverse(model))) * aNormal;
	//
	//fTextCoord = aTexCoord;
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

//in vec3 fNormal;
//in vec3 fPos;
//in vec2 fTextCoord;

in VS_OUT
{
	vec3 fNormal;
	vec3 fPos;
	vec2 fTexCoord;
	vec4 FragPosLightSpace;
} fs_in;

#define NUM_DIR_LIGHT 1

uniform Material material;
uniform DirLight dirLight[NUM_DIR_LIGHT];
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D Texture;
uniform sampler2D shadowMap;


vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace);

void main()
{
	vec3 viewDir = normalize(viewPos - fs_in.fPos);

	vec3 result = vec3(0.0);

	for (int i = 0; i < NUM_DIR_LIGHT; i++)
	{
		if (dirLight[i].isActive)
			result += calculateDirLight(dirLight[i], fs_in.fNormal, viewDir);
	}

	FragColor = vec4(result, 1.0);
};

vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	/*ambient*/
	vec3 ambient = light.ambient * texture(Texture, fs_in.fTexCoord).rgb;

	/*diffuse*/
	vec3 norm = normalize(normal);
	//vec3 lightDir = normalize(lightPos - fPos);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(Texture, fs_in.fTexCoord).rgb;

	/*specular*/
	//vec3 viewDir = normalize(viewPos - fPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	float shadows = ShadowCalculation(fs_in.FragPosLightSpace);
	vec3 result = (ambient + (1.0f - shadows) * (diffuse + specular)) * light.intensity;
	return result;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	// check whether current frag pos is in shadow
	float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

	return shadow;
}


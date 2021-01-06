#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoord;
layout(location = 3) in mat4 aInstanceModel;

out vec3 fNormal;
out vec3 fPos;
out vec2 fTextCoord;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fPos = vec3(aInstanceModel * vec4(position, 1.0));
	gl_Position = projection * view * vec4(fPos, 1.0);
	//gl_Position = projection * view * aInstanceModel * vec4(position, 1.0);
	fNormal = mat3(transpose(inverse(aInstanceModel))) * aNormal;

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
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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
};

in vec3 fNormal;
in vec3 fPos;
in vec2 fTextCoord;

uniform Material material;
uniform SpotLight light;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D Texture;


vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 viewDir = normalize(viewPos - fPos);
	
	vec3 result = calculateSpotLight(light, fNormal, fPos, viewDir);
	FragColor = vec4(result, 1.0);
};

vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	/*ambient*/
	vec3 ambient = light.ambient * texture(Texture, fTextCoord).rgb;

	/*diffuse*/
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fPos);
	//vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(Texture, fTextCoord).rgb;

	/*specular*/
	//vec3 viewDir = normalize(viewPos - fPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);


	vec3 result = (ambient + diffuse + specular);
	return result;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	/*ambient*/
	vec3 ambient = light.ambient * texture(Texture, fTextCoord).rgb;

	/*diffuse*/
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(Texture, fTextCoord).rgb;

	/*specular*/
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * material.specular;

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light._linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;


	vec3 result = (ambient + diffuse + specular);
	return result;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);


	/*ambient*/
	vec3 ambient = light.ambient * texture(Texture, fTextCoord).rgb;

	/*diffuse*/
	vec3 norm = normalize(normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(Texture, fTextCoord).rgb;

	/*specular*/
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * material.specular;

	/*soft edges*/
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = (light.cutOff - light.outerCutOff);
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	diffuse *= intensity;
	specular *= intensity;

	/*attenuation*/
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light._linear * distance + light.quadratic * (distance * distance));

	diffuse *= attenuation;
	specular *= attenuation;


	vec3 result = (ambient + diffuse + specular);
	return result;
}


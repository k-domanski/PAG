#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout(location = 3) in mat4 aInstanceModel;

out vec3 fNormal;
out vec3 fPos;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fPos = vec3(aInstanceModel * vec4(position, 1.0));
	gl_Position = projection * view * vec4(fPos, 1.0);
	//gl_Position = projection * view * aInstanceModel * vec4(position, 1.0);
	fNormal = mat3(transpose(inverse(aInstanceModel))) * aNormal;
};

#shader fragment
#version 330 core

out vec4 FragColor;
in vec3 fNormal;
in vec3 fPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objColor;
uniform vec3 viewPos;

void main()
{
	/*ambient*/
	float ambientStr = 0.1;
	vec3 ambient = ambientStr * lightColor;

	/*diffuse*/
	vec3 norm = normalize(fNormal);
	vec3 lightDir = normalize(lightPos - fPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	/*specular*/
	float specularStrenght = 0.5;
	vec3 viewDir = normalize(viewPos - fPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrenght * spec * lightColor;



	vec3 result = (ambient + diffuse + specular) * objColor;
	FragColor = vec4(result, 1.0);
};

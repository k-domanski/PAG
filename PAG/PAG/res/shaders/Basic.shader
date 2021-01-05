#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
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
in vec3 fNormal;
in vec3 fPos;
in vec2 fTextCoord;

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

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};



uniform Material material;
uniform DirLight light;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D Texture;


//vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir);
//vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//vec3 calculateSpotLight();
void main()
{
	/*ambient*/
	vec3 ambient = light.ambient * material.ambient;

	/*diffuse*/
	vec3 norm = normalize(fNormal);
	vec3 lightDir = normalize(lightPos - fPos);
	//vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);
	//vec3 diffuse = light.diffuse * (diff * texture(Texture, fTextCoord).rgb);
	
	/*specular*/
	vec3 viewDir = normalize(viewPos - fPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);



	vec3 result = (ambient + diffuse + specular) * texture(Texture, fTextCoord).rgb;
	FragColor = vec4(result, 1.0);
	//FragColor = texture(Texture, fTextCoord);
};

//vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
//{
//
//}
//
//vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//
//}


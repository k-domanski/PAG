#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aColor;
layout(location = 3) in mat4 aInstanceModel;

out vec3 fColor;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * aInstanceModel * vec4(position, 1.0);
	fColor = aColor;
};

#shader fragment
#version 330 core

out vec4 FragColor;
in vec3 fColor;

uniform vec3 lightColor;

void main()
{
	float ambientStr = 0.1;
	vec3 ambient = ambientStr * lightColor;
	vec3 result = ambient * fColor;
	FragColor = vec4(result, 1.0);
};

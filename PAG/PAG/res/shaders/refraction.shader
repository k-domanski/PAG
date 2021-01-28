#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;


out vec3 Position;
out vec3 Normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	Normal = mat3(transpose(inverse(model))) * aNormal;
	Position = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * vec4(Position, 1.0);
};

#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 Position;
in vec3 Normal;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
	float ratio = 1.0 / 2.42;//1.52;
	vec3 I = normalize(Position - cameraPos);
	vec3 R = refract(I, normalize(Normal), ratio);
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
};
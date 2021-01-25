#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTextCoord;

out vec2 textCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	textCoord = aTextCoord;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
};

#shader fragment
#version 330 core
in vec2 textCoord;
out vec4 FragColor;

uniform sampler2D Texture;
void main()
{
	FragColor = texture(Texture, textCoord);
};


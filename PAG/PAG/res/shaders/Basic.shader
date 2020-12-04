#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout(location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

out vec2 v_TexCoord;

//uniform mat4 u_MVP;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

out vec4 color;

in vec2 v_TexCoord;


uniform sampler2D texture_diffuse1;

void main()
{
	//color = u_Color;
	//color = vec4(0.2, 0.3, 0.8, 1.0);
	color = texture(texture_diffuse1, v_TexCoord);
	//color = texColor * color;
};
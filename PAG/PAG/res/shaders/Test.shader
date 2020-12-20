#shader vertex
#version 330 core

layout(location = 0) in vec4 position;



//uniform mat4 u_MVP;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * position;
};

#shader fragment
#version 330 core

out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{
	//color = u_Color;
	color = vec4(0.2, 0.3, 0.8, 1.0);
	//color = texture(texture_diffuse1, v_TexCoord);
	//color = texColor * color;
};

#shader geometry
#version 330 core
layout(triangles) in;
layout(points, max_vertices = 1) out;



void circle(vec4 position, vec2 r)
{
	for (int i = 0; i < 11; i++)
	{
		float theta = 2.0 * 3.141592 * i / 10;
		float px = r.x * cos(theta);
		float py = r.x * sin(theta);

		gl_Position = position;
		EmitVertex();
		gl_Position = position + vec4(px, py, 0.0, 0.0);
		EmitVertex();
	}
	EndPrimitive();
}
void main()
{
	//gl_Position = gl_in[0].gl_Position;
	gl_Position = position;
	EmitVertex();
	EndPrimitive();
};
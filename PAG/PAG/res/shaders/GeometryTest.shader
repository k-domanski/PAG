#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos, 1.0);
};

#shader fragment
#version 330 core

out vec4 color;


void main()
{
	color = vec4(0.0, 1.0, 0.0, 1.0);
};

#shader geometry
#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 204) out;
uniform mat4 model1;
uniform mat4 view1;
uniform mat4 projection1;
uniform int depth;

int count = 3;
void circle(vec4 position, int x)
{
	int COUNT = depth;
	float y = float(x);
	vec4 g;
	vec4 tab[100];
	for (int i = 0; i < COUNT; i++)
	{
		float theta = 2.0 * 3.141592 * i / (depth - 1);
		float px = 1.0 * cos(theta);
		float py = 1.0 * sin(theta);
		gl_Position = projection1 * view1 * model1 * (position);
		EmitVertex();
		g = vec4(py, 0.0, px, 0.0);
		vec4 pos = position + g;
		vec4 world = projection1 * view1 * model1 * pos;
		gl_Position = world;
		tab[i] = world;
		EmitVertex();

	}
	EndPrimitive();
	EndPrimitive();
	for (int i = 0; i < COUNT - 1; i++)
	{


		if (i + 1 == COUNT)
		{
		}
		else
		{
			gl_Position = tab[i];
			EmitVertex();
			gl_Position = tab[i + 1];
			EmitVertex();
			gl_Position = (tab[i] + vec4(0.0, 1.0, 0.0, 0.0));
			EmitVertex();
			gl_Position = (tab[i + 1]);
			EmitVertex();
			gl_Position = (tab[i + 1] + vec4(0.0, 1.0, 0.0, 0.0));
			EmitVertex();
			gl_Position = (tab[i] + vec4(0.0, 1.0, 0.0, 0.0));
			EmitVertex();
		}
		EndPrimitive();
	}
	

	for (int j = 0 ; j < COUNT - 1; j++)
	{
		gl_Position = (tab[j] + vec4(0.0, 1.0, 0.0, 0.0));
		EmitVertex();
		gl_Position = (tab[j + 1] + vec4(0.0, 1.0, 0.0, 0.0));
		EmitVertex();
		gl_Position = projection1 * view1 * model1 * gl_in[0].gl_Position + vec4(0.0, 1.0, 0.0, 0.0);
		gl_Position = projection1 * view1 * model1 * gl_in[0].gl_Position + vec4(0.0, 1.0, 0.0, 0.0);
		EmitVertex();
		EndPrimitive();
	}
	
}




void main()
{
	
	circle(gl_in[0].gl_Position, 1);

};
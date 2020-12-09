#shader vertex
#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aData;
layout(location = 2) in float aSeg;

out vec2 data;
out int okon;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
	data = aData;
	okon = int(aSeg);
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
layout(triangle_strip, max_vertices = 50) out;

in vec2 data[];
in int okon[];

void circle(vec4 position, vec2 r)
{
	int COUNT = okon[0];
	for (int i = 0; i < COUNT; i++)
	{
		float theta = 2.0 * 3.141592 * i / (okon[0] - 1);
		float px = r.x * cos(theta);
		float py = r.x * sin(theta);

		gl_Position = position;
		EmitVertex();
		gl_Position =  position + vec4(px, py, 0.0, 0.0);
		EmitVertex();
	}
	EndPrimitive();
}
void test(vec4 position, vec2 r)
{
	//for (int i = 0; i < 4; i++)
	//{
	for (int i = -1; i < 2; i++)
	{
		gl_Position = position + vec4(r.x, r.y * i, 0.0, 0.0);
		EmitVertex();
	}
	//gl_Position = position + vec4(r.x, r.y, 0.0, 0.0);
	//EmitVertex();
	//gl_Position = position + vec4(r.x, -r.y, 0.0, 0.0);
	//EmitVertex();
	//gl_Position = position + vec4(-r.x, r.y, 0.0, 0.0);
	//EmitVertex();
	//gl_Position = position + vec4(-r.x, -r.y, 0.0, 0.0);
	//EmitVertex();
	//}
	EndPrimitive();
}

void quad(vec4 position, vec2 pos)
{
	gl_Position = position + vec4(pos.x, pos.y, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(pos.x, -pos.y, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(-pos.x, pos.y, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(-pos.x, -pos.y, 0.0, 0.0);
	EmitVertex();
	EndPrimitive();
}

void buildHouse(vec4 position)
{
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:bottom-left
	EmitVertex();
	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);    // 2:bottom-right
	EmitVertex();
	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);    // 3:top-left
	EmitVertex();
	gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);    // 4:top-right
	EmitVertex();
	gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);    // 5:top
	EmitVertex();
	EndPrimitive();
}

void main()
{
	//gl_Position = gl_in[0].gl_Position;
	//EmitVertex();
	//EndPrimitive();
	//buildHouse(gl_in[0].gl_Position);
	circle(gl_in[0].gl_Position, data[0]);
};
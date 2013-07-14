#version 400 core

uniform vec3 Color;
in vec3 Normal;
in vec3 Vertex;

void main()
{
	gl_FrontColor = vec4(Color, 1.0);
	gl_Position = vec4(Vertex, 1.0);
}

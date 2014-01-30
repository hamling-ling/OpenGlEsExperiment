#version 400 core

in vec3 Vertex;

uniform mat4 modelViewProjectionMatrix;

void main()
{
	gl_Position = modelViewProjectionMatrix * vec4(Vertex, 1.0);
}

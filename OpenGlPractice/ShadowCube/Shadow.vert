#version 400 core

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 viewMatrix;
uniform vec4 lightPosition;
uniform mat4 shadowModelViewProjectionMatrix;

in vec3 Normal;
in vec3 Vertex;

out vec3 normal;
out vec3 position;
out vec3 light;
out vec4 shadowPosition;

void main(void)
{
	normal = normalize(mat3(modelViewMatrix) * Normal);
	position = (modelViewMatrix * vec4(Vertex, 1.0)).xyz;
	light = (viewMatrix * lightPosition).xyz;
	shadowPosition = shadowModelViewProjectionMatrix * vec4(Vertex, 1.0);

	gl_Position = modelViewProjectionMatrix * vec4(Vertex, 1.0);
}

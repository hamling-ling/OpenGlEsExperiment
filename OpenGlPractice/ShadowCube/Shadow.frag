#version 400 core

uniform vec4 Ld;
uniform vec4 Ls;
uniform vec4 La;
uniform vec4 Kd;
uniform vec4 Ks;
uniform vec4 Ka;
uniform float shininess;
uniform sampler2DShadow shadowTexture;

in vec3 normal;
in vec3 position;
in vec3 light;
in vec4 shadowPosition;

layout (location = 0) out vec4 FragColor;

void main(void)
{
	vec4 ambient = Ka * La;

	vec3 N = normalize(normal);
	vec3 P = position.xyz;
	vec3 L = normalize(light - P);
	float diffuseLighting = max(dot(L, N), 0);
	vec4 diffuse = Kd * diffuseLighting * Ld;

	vec3 V = normalize(-P);
	vec3 H = normalize(L + V);
	float specularLighting = pow(max(dot(H, N), 0), shininess);
	if (diffuseLighting <= 0.0) {
		specularLighting = 0.0;
	}
	vec4 specular = Ks * specularLighting * Ls;

	float inLight = textureProj(shadowTexture, shadowPosition);

	FragColor = ambient + (diffuse + specular) * inLight;
}

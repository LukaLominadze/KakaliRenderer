#version 460 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

struct GlobalLight {
	vec3 ambient;
	float intensity;
};

uniform GlobalLight globalLight;

void main() {
	vec4 ambient = vec4(globalLight.ambient * globalLight.intensity, 1.0);
    FragColor = ambient * texture(skybox, TexCoords);
}
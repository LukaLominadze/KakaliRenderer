#version 460 core

in vec2 oTexCoord;

out vec4 FragColor;

uniform sampler2D texture_diffuse0;

void main() {
	FragColor = texture(texture_diffuse0, oTexCoord);
}
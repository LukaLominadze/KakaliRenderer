#version 460 core

in vec3 oWorldPos;

uniform vec3 lightWorldPos;

out float lightToPixelDistance;

void main() {
	vec3 lightToVertex = oWorldPos - lightWorldPos;
	lightToPixelDistance = length(lightToVertex);
}
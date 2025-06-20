#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 modelViewProjection;

void main() {
	TexCoords = aPos;
	gl_Position = modelViewProjection * vec4(aPos, 1.0);
}
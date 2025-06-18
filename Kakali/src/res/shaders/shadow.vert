#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 modelViewProjection;
uniform mat4 worldPos;

out vec3 oWorldPos;

void main() {
    oWorldPos = (worldPos * vec4(aPos, 1.0)).xyz;
	gl_Position = modelViewProjection * vec4(aPos, 1.0);
}
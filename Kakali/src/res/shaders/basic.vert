#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec2 oTexCoord;

uniform mat4 modelViewProjection;

void main()
{
    oTexCoord = aTexCoord;
    gl_Position = modelViewProjection * vec4(aPos, 1.0);
}
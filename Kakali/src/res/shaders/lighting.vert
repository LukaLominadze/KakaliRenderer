#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec3 oPos;
out vec2 oTexCoord;
out vec3 oNormal;
out vec4 oDirShadowProjPos;
out vec4 oSpotShadowProjPos;

uniform mat4 modelViewProjection;
uniform mat4 dirShadowViewProjection;
uniform mat4 spotShadowViewProjection;

void main()
{
    oPos = aPos;
    oTexCoord = aTexCoord;
    oNormal = aNormal;
    oDirShadowProjPos = dirShadowViewProjection * vec4(aPos, 1.0);
    oSpotShadowProjPos = spotShadowViewProjection * vec4(aPos, 1.0);
    gl_Position = modelViewProjection * vec4(aPos, 1.0);
}
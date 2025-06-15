#version 330 core

layout (location=0) in vec3 position;

layout (location=2) in vec3 vertexNormal;
layout (location=8) in vec2 texCoord;

out vec2 ComputedTexCoord;
out vec3 ComputedVertexNormal;
out vec3 ComputedVertexPos;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    vec4 mvPos = modelViewMatrix * vec4(position, 1.0);
    gl_Position = projectionMatrix * mvPos;
    ComputedTexCoord = texCoord;
    ComputedVertexNormal = normalize(modelViewMatrix * vec4(vertexNormal, 0.0)).xyz;
    ComputedVertexPos = mvPos.xyz;
}

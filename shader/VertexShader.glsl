#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 uv;

out vec3 vertexPosition;
out vec3 vertexNormal;
out vec3 vertexTangent;
out vec3 vertexBitangent;
out vec2 vertexUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vertexPosition = vec3(model * vec4(position, 1));
    vertexNormal = vec3(model * vec4(normal, 0));
    vertexTangent = vec3(model * vec4(tangent, 0));
    vertexBitangent = vec3(model * vec4(bitangent, 0));
    vertexUV = uv;

    gl_Position = projection * view * model * vec4(position, 1);
}
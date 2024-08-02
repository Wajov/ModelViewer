#version 330 core

const int MAX_BONES = 100;
const int MAX_BONES_INFLUENCE = 4;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 uv;
layout(location = 5) in ivec4 boneIds;
layout(location = 6) in vec4 boneWeights;

out vec3 vertexPosition;
out vec3 vertexNormal;
out vec3 vertexTangent;
out vec3 vertexBitangent;
out vec2 vertexUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int animationExist;
uniform mat4 finalBoneMatrices[MAX_BONES];

void main() {
    vec4 sumPosition = vec4(0);
    vec4 sumNormal = vec4(0);
    vec4 sumTangent = vec4(0);
    vec4 sumBitangent = vec4(0);
    if (animationExist == 1) {
        for (int i = 0; i < MAX_BONES_INFLUENCE; i++) {
            if (boneIds[i] == -1) {
                continue;
            }
            if (boneIds[i] >= MAX_BONES) {
                sumPosition = vec4(position, 1);
                sumNormal = vec4(normal, 0);
                sumTangent = vec4(tangent, 0);
                sumBitangent = vec4(bitangent, 0);
                break;
            }

            sumPosition += boneWeights[i] * finalBoneMatrices[boneIds[i]] * vec4(position, 1);
            sumNormal += boneWeights[i] * finalBoneMatrices[boneIds[i]] * vec4(normal, 0);
            sumTangent += boneWeights[i] * finalBoneMatrices[boneIds[i]] * vec4(tangent, 0);
            sumBitangent += boneWeights[i] * finalBoneMatrices[boneIds[i]] * vec4(bitangent, 0);
        }
    } else {
        sumPosition = vec4(position, 1);
        sumNormal = vec4(normal, 0);
        sumTangent = vec4(tangent, 0);
        sumBitangent = vec4(bitangent, 0);
    }

    // sumPosition = vec4(position, 1);
    vertexPosition = vec3(model * sumPosition);
    vertexNormal = vec3(model * sumNormal);
    vertexTangent = vec3(model * sumTangent);
    vertexBitangent = vec3(model * sumBitangent);
    vertexUV = uv;

    gl_Position = projection * view * model * sumPosition;
}
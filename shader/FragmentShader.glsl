#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec3 vertexTangent;
in vec3 vertexBitangent;
in vec2 vertexUV;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;
uniform int ambientExist;
uniform int diffuseExist;
uniform int specularExist;
uniform int normalExist;
uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;
uniform float lightPower;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main() {
    float distance = length(lightPosition - vertexPosition);

    vec3 ambient = ambientExist == 1 ? ambientColor * vec3(texture(ambientTexture, vertexUV)) : ambientColor;

    vec3 N = normalize(normalExist == 1 ? mat3(vertexTangent, vertexBitangent, vertexNormal) * (vec3(texture(normalTexture, vertexUV)) * 2 - vec3(1)) : vertexNormal);
    vec3 L = normalize(lightPosition - vertexPosition);
    vec3 diffuse = (diffuseExist == 1 ? diffuseColor * vec3(texture(diffuseTexture, vertexUV)) : diffuseColor) * clamp(dot(N, L), 0, 1) * lightPower / (distance * distance);

    vec3 V = normalize(cameraPosition - vertexPosition);
    vec3 H = normalize(L + V);
    vec3 specular = (specularExist == 1 ? specularColor * vec3(texture(specularTexture, vertexUV)) : specularColor) * pow(clamp(dot(N, H), 0, 1), shininess) * lightPower / (distance * distance);

    gl_FragColor = vec4(ambient + diffuse + specular, 1);
}
#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec3 vertexTangent;
in vec3 vertexBitangent;
in vec2 vertexUV;

uniform int ambientExist;
uniform sampler2D ambientTexture;
uniform int diffuseExist;
uniform sampler2D diffuseTexture;
uniform int specularExist;
uniform sampler2D specularTexture;
uniform int normalExist;
uniform sampler2D normalTexture;
uniform vec3 lightDirection;
uniform vec3 cameraPosition;

void main() {
    vec3 ambientColor, diffuseColor, specularColor;
    if (ambientExist == 1 || diffuseExist == 1 || specularExist == 1) {
        ambientColor = ambientExist == 1 ? vec3(texture(ambientTexture, vertexUV)) : vec3(0, 0, 0);
        diffuseColor = diffuseExist == 1 ? vec3(texture(diffuseTexture, vertexUV)) : vec3(0, 0, 0);
        specularColor = specularExist == 1 ? vec3(texture(specularTexture, vertexUV)) : vec3(0, 0, 0);
    } else {
        ambientColor = 0.1 * vec3(1, 1, 1);
        diffuseColor = 0.6 * vec3(1, 1, 1);
        specularColor = 0.3 * vec3(1, 1, 1);
    }

    vec3 ambient = ambientColor;

    vec3 N = normalize(normalExist == 1 ? mat3(vertexTangent, vertexBitangent, vertexNormal) * (vec3(texture(normalTexture, vertexUV)) * 2 - vec3(1, 1, 1)) : vertexNormal);
    vec3 L = normalize(-lightDirection);
    vec3 diffuse = diffuseColor * clamp(dot(N, L), 0, 1);

    vec3 V = normalize(cameraPosition - vertexPosition);
    vec3 H = normalize(L + V);
    vec3 specular = specularColor * pow(clamp(dot(N, H), 0, 1), 5);

    gl_FragColor = vec4(ambient + diffuse + specular, 1);
}
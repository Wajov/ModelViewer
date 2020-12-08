#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUV;

uniform sampler2D textureSampler;
uniform vec3 lightDirection;
uniform vec3 cameraPosition;

void main() {
    vec3 a, b;
    float cosine;

    vec3 color = vec3(texture(textureSampler, vertexUV));
    vec3 ambientColor = vec3(0.1, 0.1, 0.1) * color;
    vec3 diffuseColor = vec3(0.6, 0.6, 0.6) * color;
    vec3 specularColor = vec3(0.3, 0.3, 0.3);

    vec3 ambient = ambientColor;

    a = normalize(vertexNormal);
    b = normalize(-lightDirection);
    cosine = clamp(dot(a, b), 0, 1);
    vec3 diffuse = diffuseColor * cosine;

    a = normalize(reflect(-b, a));
    b = normalize(cameraPosition - vertexPosition);
    cosine = clamp(dot(a, b), 0, 1);
    vec3 specular = specularColor * pow(cosine, 5);

    gl_FragColor = vec4(ambient + diffuse + specular, 1);
}
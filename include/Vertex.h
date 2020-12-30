#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

class Vertex {
public:
    glm::vec3 position, normal, tangent, bitangent;
    glm::vec2 uv;
    Vertex(glm::vec3 &position, glm::vec3 &normal);
    Vertex(glm::vec3 &position, glm::vec3 &normal, glm::vec3 &tangent, glm::vec3 &bitangent, glm::vec2 &uv);
    ~Vertex();
};

#endif
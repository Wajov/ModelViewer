#include "Vertex.h"

Vertex::Vertex(glm::vec3 &position, glm::vec3 &normal) {
    this->position = position;
    this->normal = normal;
}

Vertex::Vertex(glm::vec3 &position, glm::vec3 &normal, glm::vec3 &tangent, glm::vec3 &bitangent, glm::vec2 &uv) {
    this->position = position;
    this->normal = normal;
    this->tangent = tangent;
    this->bitangent = bitangent;
    this->uv = uv;
}

Vertex::~Vertex() {}
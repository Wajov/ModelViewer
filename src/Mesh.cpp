#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, Texture &ambient, Texture &diffuse, Texture &specular, Texture &normal) {
    this->vertices = vertices;
    this->indices = indices;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->normal = normal;
}

Mesh::~Mesh() {}

void Mesh::coordinateRange(float &xMin, float &xMax, float &yMin, float &yMax, float &zMin, float &zMax) {
    xMin = yMin = zMin = FLT_MAX;
    xMax = yMax = zMax = -FLT_MAX;
    for (Vertex &vertex : vertices) {
        xMin = std::min(xMin, vertex.position.x);
        xMax = std::max(xMax, vertex.position.x);
        yMin = std::min(yMin, vertex.position.y);
        yMax = std::max(yMax, vertex.position.y);
        zMin = std::min(zMin, vertex.position.z);
        zMax = std::max(zMax, vertex.position.z);
    }
}

void Mesh::recenter(glm::vec3 &center) {
    for (Vertex &vertex : vertices)
        vertex.position -= center;
}

void Mesh::bind() {
    unsigned vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &(this->vertices[0]), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &(this->indices[0]), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindVertexArray(0);
}

void Mesh::render(Shader &shader) {
    shader.setInt("ambientExist", ambient.getExist());
    shader.setInt("ambientTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ambient.getId());

    shader.setInt("diffuseExist", diffuse.getExist());
    shader.setInt("diffuseTexture", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, diffuse.getId());

    shader.setInt("specularExist", specular.getExist());
    shader.setInt("specularTexture", 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, specular.getId());

    shader.setInt("normalExist", normal.getExist());
    shader.setInt("normalTexture", 3);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, normal.getId());

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
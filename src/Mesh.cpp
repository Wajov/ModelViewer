#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
}

void Mesh::coordinateRange(float &xMin, float &xMax, float &yMin, float &yMax, float &zMin, float &zMax) {
    xMin = yMin = zMin = 1e38f;
    xMax = yMax = zMax = -1e38f;
    for (unsigned int i = 0; i < vertices.size(); i++) {
        xMin = std::min(xMin, vertices[i].position.x);
        xMax = std::max(xMax, vertices[i].position.x);
        yMin = std::min(yMin, vertices[i].position.y);
        yMax = std::max(yMax, vertices[i].position.y);
        zMin = std::min(zMin, vertices[i].position.z);
        zMax = std::max(zMax, vertices[i].position.z);
    }
}

void Mesh::recenter(glm::vec3 center) {
    for (int i = 0; i < vertices.size(); i++)
        vertices[i].position -= center;
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
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tan));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitan));

    glBindVertexArray(0);
}

void Mesh::render(Shader shader) {
    unsigned int diffuse = 0, specular = 0, normal = 0, height = 0;
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number, name = textures[i].type;

        if (name == "texture_diffuse")
            number = std::to_string(++diffuse);
        else if (name == "texture_specular")
            number = std::to_string(++specular);
        else if (name == "texture_normal")
            number = std::to_string(++normal);
        else if (name == "texture_height")
            number = std::to_string(++height);

        shader.setInt((name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
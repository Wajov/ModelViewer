#include <iostream>

#include <glad/glad.h>

#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, Vector3f& ambientColor,
    Vector3f& diffuseColor, Vector3f& specularColor, float shininess, Image* ambientImage, Image* diffuseImage,
    Image* specularImage, Image* normalImage) :
    vertices(vertices),
    indices(indices),
    ambientColor(ambientColor),
    diffuseColor(diffuseColor),
    specularColor(specularColor),
    shininess(shininess),
    ambientImage(ambientImage),
    diffuseImage(diffuseImage),
    specularImage(specularImage),
    normalImage(normalImage) {}

Mesh::~Mesh() {
    delete ambientImage;
    delete diffuseImage;
    delete specularImage;
    delete normalImage;
}

void Mesh::processTexture(unsigned int texture, const Image* image) const {
    glBindTexture(GL_TEXTURE_2D, texture);
    unsigned int format;
    switch (image->getChannels()) {
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            std::cerr << "Unsupported image format!" << std::endl;
            return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, image->getWidth(), image->getHeight(), 0, format, GL_UNSIGNED_BYTE,
        image->getData());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Mesh::bind() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, bitangent)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));

    glBindVertexArray(0);

    if (ambientImage != nullptr) {
        glGenTextures(1, &ambientTexture);
        processTexture(ambientTexture, ambientImage);
    }
    if (diffuseImage != nullptr) {
        glGenTextures(1, &diffuseTexture);
        processTexture(diffuseTexture, diffuseImage);
    }
    if (specularImage != nullptr) {
        glGenTextures(1, &specularTexture);
        processTexture(specularTexture, specularImage);
    }
    if (normalImage != nullptr) {
        glGenTextures(1, &normalTexture);
        processTexture(normalTexture, normalImage);
    }
}

void Mesh::render(const Shader* shader) const {
    shader->setVec3("ambientColor", ambientColor);
    shader->setVec3("diffuseColor", diffuseColor);
    shader->setVec3("specularColor", specularColor);
    shader->setFloat("shininess", shininess);

    shader->setInt("ambientExist", ambientImage != nullptr);
    if (ambientImage != nullptr) {
        shader->setInt("ambientTexture", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ambientTexture);
    }

    shader->setInt("diffuseExist", diffuseImage != nullptr);
    if (diffuseImage != nullptr) {
        shader->setInt("diffuseTexture", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    }

    shader->setInt("specularExist", specularImage != nullptr);
    if (specularImage != nullptr) {
        shader->setInt("specularTexture", 2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, specularTexture);
    }

    shader->setInt("normalExist", normalImage != nullptr);
    if (normalImage != nullptr) {
        shader->setInt("normalTexture", 3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, normalTexture);
    }

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
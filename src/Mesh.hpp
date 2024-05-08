#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include "TypeHelper.hpp"
#include "Vertex.hpp"
#include "Shader.hpp"
#include "Image.hpp"

class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Vector3f ambientColor, diffuseColor, specularColor;
    Image* ambientImage, * diffuseImage, * specularImage, * normalImage;
    float shininess;
    unsigned int vao, vbo, ebo, ambientTexture, diffuseTexture, specularTexture, normalTexture;
    void processTexture(unsigned int texture, const Image* image) const;

public:
    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, Vector3f& ambientColor,
        Vector3f& diffuseColor, Vector3f& specularColor, float shininess, Image* ambientImage, Image* diffuseImage,
        Image* specularImage, Image* normalImage);
    ~Mesh();
    void bind();
    void render(const Shader* shader) const;
};

#endif
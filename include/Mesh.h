#ifndef MESH_H
#define MESH_H

#include <bits/stdc++.h>

#include <glad/glad.h>

#include "Shader.h"

struct Vertex {
    glm::vec3 position, normal, tan, bitan;
    glm::vec2 uv;
};

struct Texture {
    unsigned int id;
    std::string type, path;
};

class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int vao;

public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void coordinateRange(float &xMin, float &xMax, float &yMin, float &yMax, float &zMin, float &zMax);
    void recenter(glm::vec3 center);
    void bind();
    void render(Shader shader);
};

#endif
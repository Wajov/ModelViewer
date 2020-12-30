#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"

class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Texture ambient, diffuse, specular, normal;
    unsigned int vao;

public:
    Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, Texture &ambient, Texture &diffuse, Texture &specular, Texture &normal);
    ~Mesh();
    void coordinateRange(float &xMin, float &xMax, float &yMin, float &yMax, float &zMin, float &zMax);
    void recenter(glm::vec3 &center);
    void bind();
    void render(Shader &shader);
};

#endif
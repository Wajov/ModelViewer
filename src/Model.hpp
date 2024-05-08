#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector>

#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/material.h>

#include "TypeHelper.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Image.hpp"

class Model {
private:
    std::vector<Mesh*> meshes;
    Shader* shader;
    void processNode(aiNode *node, const aiScene *scene, const std::string& directory);
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene, const std::string& directory);
    Image* processTexture(aiMaterial *material, aiTextureType type, const std::string& directory);

public:
    Model(const std::string& path);
    ~Model();
    void bind();
    void render(const Matrix4x4f& model, const Matrix4x4f& view, const Matrix4x4f& projection,
        const Vector3f& cameraPosition, const Vector3f& lightDirection) const;
};

#endif
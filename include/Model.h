#ifndef MODEL_H
#define MODEL_H

#include <algorithm>
#include <cfloat>
#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Vertex.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"

class Model {
private:
    std::vector<Texture> textures;
    std::vector<Mesh> meshes;
    std::string directory;
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    Texture processMaterial(aiMaterial *material, aiTextureType type);
    unsigned int processTexture(std::string &name);

public:
    Model(const std::string &path);
    ~Model();
    void render(Shader &shader);
};

#endif
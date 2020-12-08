#ifndef MODEL_H
#define MODEL_H

#include <bits/stdc++.h>

#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class Model {
private:
    std::vector<Texture> textures;
    std::vector<Mesh> meshes;
    std::string directory;
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string name);
    unsigned int loadTextureFromFile(const char *path, const std::string &directory);

public:
    Model(const std::string path);
    void render(Shader shader);
};

#endif
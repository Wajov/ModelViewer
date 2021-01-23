#ifndef MODEL_H
#define MODEL_H

#include <algorithm>
#include <cfloat>
#include <iostream>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QVector2D>
#include <QVector3D>
#include <QImage>
#include <QOpenGLShaderProgram>

#include "Vertex.h"
#include "Mesh.h"

class Model {
private:
    std::vector<Mesh> meshes;
    void processNode(aiNode *node, const aiScene *scene, std::string &directory);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene, std::string &directory);
    QImage processTexture(aiMaterial *material, aiTextureType type, std::string &directory);

public:
    Model();
    Model(const std::string &path);
    ~Model();
    void bind(QOpenGLShaderProgram &program);
    void render(QOpenGLShaderProgram &program);
};

#endif
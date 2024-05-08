#include <algorithm>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Model.hpp"

Model::Model(const std::string& path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
        aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags&  AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Failed to load model:" << std::endl << importer.GetErrorString() << std::endl;
        return;
    }

    std::string directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene, directory);
}

Model::~Model() {
    for (const Mesh* mesh : meshes) {
        delete mesh;
    }
    delete shader;
}

void Model::processNode(aiNode *node, const aiScene *scene, const std::string& directory) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene, directory));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, directory);
    }
}

Mesh* Model::processMesh(aiMesh *mesh, const aiScene *scene, const std::string& directory) {
    std::vector<Vertex> vertices;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vector3f position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        Vector3f normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0] != nullptr) {
            Vector3f tangent(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            Vector3f bitangent(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
            Vector2f uv(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            vertices.push_back(Vertex(position, normal, tangent, bitangent, uv));
        } else {
            vertices.push_back(Vertex(position, normal));
        }
    }

    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
            indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    aiColor3D ambient, diffuse, specular;
    float shininess;
    material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    material->Get(AI_MATKEY_SHININESS, shininess);
    Vector3f ambientColor = Vector3f(ambient.r, ambient.g, ambient.b);
    Vector3f diffuseColor = Vector3f(diffuse.r, diffuse.g, diffuse.b);
    Vector3f specularColor = Vector3f(specular.r, specular.g, specular.b);
    shininess = std::max(shininess, 0.0f);

    Image* ambientImage = processTexture(material, aiTextureType_AMBIENT, directory);
    Image* diffuseImage = processTexture(material, aiTextureType_DIFFUSE, directory);
    Image* specularImage = processTexture(material, aiTextureType_SPECULAR, directory);
    Image* normalImage = processTexture(material, aiTextureType_HEIGHT, directory);

    if (ambient.IsBlack() && diffuse.IsBlack() && specular.IsBlack() && ambientImage == nullptr &&
        diffuseImage == nullptr && specularImage == nullptr) {
        ambientColor = Vector3f(0.1f, 0.1f, 0.1f);
        diffuseColor = Vector3f(0.6f, 0.6f, 0.6f);
        specularColor = Vector3f(0.3f, 0.3f, 0.3f);
    }

    return new Mesh(vertices, indices, ambientColor, diffuseColor, specularColor, shininess, ambientImage, diffuseImage,
        specularImage, normalImage);
}

Image* Model::processTexture(aiMaterial *material, aiTextureType type, const std::string& directory) {
    if (material->GetTextureCount(type) > 0) {
        aiString nameTemp;
        material->GetTexture(type, 0, &nameTemp);
        std::string filename = nameTemp.C_Str();
        return new Image(directory + "/" + filename);
    } else {
        return nullptr;
    }
}

void Model::bind() {
    shader = new Shader("shader/Vertex.glsl", "shader/Fragment.glsl");
    for (Mesh* mesh : meshes) {
        mesh->bind();
    }
}

void Model::render(const Matrix4x4f& model, const Matrix4x4f& view, const Matrix4x4f& projection,
    const Vector3f& cameraPosition, const Vector3f& lightDirection) const {
    shader->use();
    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setVec3("color", Vector3f(0.6f, 0.7f, 1.0f));
    shader->setVec3("cameraPosition", cameraPosition);
    shader->setVec3("lightDirection", lightDirection);

    for (const Mesh* mesh : meshes) {
        mesh->render(shader);
    }
}
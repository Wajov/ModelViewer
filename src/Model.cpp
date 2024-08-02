#include <algorithm>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Model.hpp"

Model::Model(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
        aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
    if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) {
        std::cerr << "Failed to load model:" << std::endl << importer.GetErrorString() << std::endl;
        return;
    }

    std::string directory = path.substr(0, path.find_last_of('/'));
    ProcessNode(scene->mRootNode, scene, directory);
}

Model::~Model() {
    for (const Mesh* mesh : meshes) {
        delete mesh;
    }
    delete shader;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, const std::string& directory) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        meshes.push_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene, directory));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, directory);
    }
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory) {
    std::vector<Vertex> vertices;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vector3f position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        Vector3f normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0] != nullptr) {
            Vector3f tangent(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            Vector3f bitangent(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
            Vector2f uv(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            vertices.emplace_back(position, normal, tangent, bitangent, uv);
        } else {
            vertices.emplace_back(position, normal);
        }
    }

    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
            indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

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

    Image* ambientImage = ProcessTexture(material, aiTextureType_AMBIENT, directory);
    Image* diffuseImage = ProcessTexture(material, aiTextureType_DIFFUSE, directory);
    Image* specularImage = ProcessTexture(material, aiTextureType_SPECULAR, directory);
    Image* normalImage = ProcessTexture(material, aiTextureType_HEIGHT, directory);

    if (ambient.IsBlack() && diffuse.IsBlack() && specular.IsBlack() && ambientImage == nullptr &&
        diffuseImage == nullptr && specularImage == nullptr) {
        ambientColor = Vector3f(0.1f, 0.1f, 0.1f);
        diffuseColor = Vector3f(0.6f, 0.6f, 0.6f);
        specularColor = Vector3f(0.3f, 0.3f, 0.3f);
    }

    ProcessBones(mesh, scene, vertices);

    return new Mesh(vertices, indices, ambientColor, diffuseColor, specularColor, shininess, ambientImage, diffuseImage,
        specularImage, normalImage);
}

Image* Model::ProcessTexture(aiMaterial* material, aiTextureType type, const std::string& directory) {
    if (material->GetTextureCount(type) > 0) {
        aiString nameTemp;
        material->GetTexture(type, 0, &nameTemp);
        std::string filename = nameTemp.C_Str();
        return new Image(directory + "/" + filename);
    } else {
        return nullptr;
    }
}

void Model::ProcessBones(aiMesh* mesh, const aiScene* scene, std::vector<Vertex>& vertices) {
    for (int i = 0; i < mesh->mNumBones; i++) {
        std::string boneName = mesh->mBones[i]->mName.C_Str();
        if (boneInfos.find(boneName) == boneInfos.end()) {
            aiMatrix4x4 offset = mesh->mBones[i]->mOffsetMatrix;
            BoneInfo boneInfo;
            boneInfo.index = boneInfos.size();
            boneInfo.offset << offset.a1, offset.a2, offset.a3, offset.a4, offset.b1, offset.b2, offset.b3, offset.b4,
                offset.c1, offset.c2, offset.c3, offset.c4, offset.d1, offset.d2, offset.d3, offset.d4;
            boneInfos[boneName] = boneInfo;
        }
        int boneId = boneInfos[boneName].index;

        aiVertexWeight* weights = mesh->mBones[i]->mWeights;
        for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
            int index = weights[j].mVertexId;
            float weight = weights[j].mWeight;
            for (int k = 0; k < Vertex::MAX_BONE_INFLUENCE; k++) {
                if (vertices[index].boneIds[k] < 0) {
                    vertices[index].boneIds[k] = boneId;
                    vertices[index].boneWeights[k] = weight;
                }
            }
        }
    }
}

std::map<std::string, BoneInfo>& Model::GetBoneInfos() {
    return boneInfos;
}

void Model::Bind() {
    shader = new Shader("shader/Vertex.glsl", "shader/Fragment.glsl");
    for (Mesh* mesh : meshes) {
        mesh->Bind();
    }
}

void Model::SetAnimationExist(bool exist) const {
    shader->SetInt("animationExist", exist);
}

void Model::SetFinalBoneMatrices(const std::vector<Matrix4x4f>& finalBoneMatrices) const {
    for (int i = 0; i < finalBoneMatrices.size(); i++) {
        shader->SetMat4("finalBoneMatrices[" + std::to_string(i) + "]", finalBoneMatrices[i]);
    }
}

void Model::Render(const Matrix4x4f& model, const Matrix4x4f& view, const Matrix4x4f& projection,
    const Vector3f& cameraPosition, const Vector3f& lightDirection) const {
    shader->Use();
    shader->SetMat4("model", model);
    shader->SetMat4("view", view);
    shader->SetMat4("projection", projection);
    shader->SetVec3("color", Vector3f(0.6f, 0.7f, 1.0f));
    shader->SetVec3("cameraPosition", cameraPosition);
    shader->SetVec3("lightDirection", lightDirection);

    for (const Mesh* mesh : meshes) {
        mesh->Render(shader);
    }
}
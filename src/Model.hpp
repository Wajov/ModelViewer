#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector>
#include <map>

#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/material.h>

#include "TypeHelper.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Image.hpp"
#include "BoneInfo.hpp"

class Model {
private:
    std::vector<Mesh*> meshes;
    Shader* shader;
    std::map<std::string, BoneInfo> boneInfos;
    void ProcessNode(aiNode* node, const aiScene* scene, const std::string& directory);
    Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory);
    Image* ProcessTexture(aiMaterial* material, aiTextureType type, const std::string& directory);
    void ProcessBones(aiMesh* mesh, const aiScene* scene, std::vector<Vertex>& vertices);

public:
    Model(const std::string& path);
    ~Model();
    std::map<std::string, BoneInfo>& GetBoneInfos();
    void Bind();
    void SetAnimationExist(bool exist) const;
    void SetFinalBoneMatrices(const std::vector<Matrix4x4f>& finalBoneMatrices) const;
    void Render(const Matrix4x4f& model, const Matrix4x4f& view, const Matrix4x4f& projection,
        const Vector3f& cameraPosition, const Vector3f& lightDirection) const;
};

#endif
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/anim.h>

#include "Animation.hpp"

Animation::Animation(const std::string& path, Model* model) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
        aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
    if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) {
        std::cerr << "Failed to load model:" << std::endl << importer.GetErrorString() << std::endl;
        return;
    }

    aiAnimation* animation = scene->mAnimations[0];
    duration = animation->mDuration;
    ticksPerSecond = animation->mTicksPerSecond;
    root = ProcessNode(scene->mRootNode);
    ProcessBones(animation, model);
}

Animation::~Animation() {
    delete root;
    for (const auto& [name, bone] : bones) {
        delete bone;
    }
}

BoneNode* Animation::ProcessNode(const aiNode* node) {
    BoneNode* boneNode = new BoneNode(node->mName.C_Str(), node->mTransformation);
    for (int i = 0; i < node->mNumChildren; i++) {
        boneNode->AddChild(ProcessNode(node->mChildren[i]));
    }
    return boneNode;
}

void Animation::ProcessBones(const aiAnimation* animation, Model* model) {
    std::map<std::string, BoneInfo>& boneInfos = model->GetBoneInfos();
    for (int i = 0; i < animation->mNumChannels; i++) {
        aiNodeAnim* nodeAnimation = animation->mChannels[i];
        std::string boneName = nodeAnimation->mNodeName.C_Str();
        if (boneInfos.find(boneName) == boneInfos.end()) {
            BoneInfo boneInfo;
            boneInfo.index = boneInfos.size();
            boneInfos[boneName] = boneInfo;
        }
        bones[boneName] = new Bone(boneInfos[boneName], nodeAnimation);
    }
    finalBoneMatrices.resize(boneInfos.size());
}

void Animation::UpdateFinalBoneMatrices(const BoneNode* node, const Matrix4x4f& transform) {
    std::string boneName = node->GetName();

    Matrix4x4f localTransform = node->GetTransform();
    auto iter = bones.find(boneName);
    Bone* bone = nullptr;
    if (iter != bones.end()) {
        bone = iter->second;
        localTransform = bone->GetTransform(time);
    }

    Matrix4x4f globalTransform = transform * localTransform;
    if (bone != nullptr) {
        BoneInfo boneInfo = bone->GetInfo();
        finalBoneMatrices[boneInfo.index] = globalTransform * boneInfo.offset;
    }

    const std::vector<BoneNode*>& children = node->GetChildren();
    for (const BoneNode* child : children) {
        UpdateFinalBoneMatrices(child, globalTransform);
    }
}


std::vector<Matrix4x4f> Animation::GetFinalBoneMatrices(float time) {
    this->time = fmod(time * ticksPerSecond, duration);
    UpdateFinalBoneMatrices(root, Matrix4x4f::Identity());
    return finalBoneMatrices;
}
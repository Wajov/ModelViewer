#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <cmath>
#include <string>
#include <vector>
#include <map>

#include "TypeHelper.hpp"
#include "Model.hpp"
#include "BoneInfo.hpp"
#include "BoneNode.hpp"
#include "Bone.hpp"

class Animation {
private:
    float duration, ticksPerSecond, time;
    BoneNode* root;
    std::map<std::string, Bone*> bones;
    std::vector<Matrix4x4f> finalBoneMatrices;
    BoneNode* ProcessNode(const aiNode* node);
    void ProcessBones(const aiAnimation* animation, Model* model);
    void UpdateFinalBoneMatrices(const BoneNode* node, const Matrix4x4f& transform);

public:
    Animation(const std::string& path, Model* model);
    ~Animation();
    std::vector<Matrix4x4f> GetFinalBoneMatrices(float time);
};

#endif
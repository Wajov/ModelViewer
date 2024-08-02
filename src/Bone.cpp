#include "Bone.hpp"
#include "TransformationHelper.hpp"

Bone::Bone(const BoneInfo& info, const aiNodeAnim* animation) :
    info(info) {
    for (int i = 0; i < animation->mNumPositionKeys; i++) {
        aiVector3D position = animation->mPositionKeys[i].mValue;
        float time = animation->mPositionKeys[i].mTime;
        positions.emplace_back(Vector3f(position.x, position.y, position.z), time);
    }
    std::sort(positions.begin(), positions.end());

    for (int i = 0; i < animation->mNumRotationKeys; i++) {
        aiQuaternion rotation = animation->mRotationKeys[i].mValue;
        float time = animation->mRotationKeys[i].mTime;
        rotations.emplace_back(Quaternion(rotation.w, rotation.x, rotation.y, rotation.z), time);
    }
    std::sort(rotations.begin(), rotations.end());

    for (int i = 0; i < animation->mNumScalingKeys; i++) {
        aiVector3D scaling = animation->mScalingKeys[i].mValue;
        float time = animation->mScalingKeys[i].mTime;
        scalings.emplace_back(Vector3f(scaling.x, scaling.y, scaling.z), time);
    }
    std::sort(scalings.begin(), scalings.end());
}

Bone::~Bone() {}

BoneInfo Bone::GetInfo() const {
    return info;
}

float Bone::GetFactor(float a, float b, float x) const {
    return (x - a) / (b - a);
}

Matrix4x4f Bone::GetPosition(float time) const {
    if (positions.size() == 1) {
        return Translate(positions.front().position);
    }

    auto jter = std::upper_bound(positions.begin(), positions.end(), KeyPosition(Vector3f::Identity(), time));
    auto iter = jter - 1;
    float t = GetFactor(iter->time, jter->time, time);
    return Translate((1.0f - t) * iter->position + t * jter->position);
}

Matrix4x4f Bone::GetRotation(float time) const {
    if (rotations.size() == 1) {
        return Rotate(rotations.front().rotation);
    }

    auto jter = std::upper_bound(rotations.begin(), rotations.end(), KeyRotation(Quaternion::Identity(), time));
    auto iter = jter - 1;
    float t = GetFactor(iter->time, jter->time, time);
    return Rotate(iter->rotation.slerp(t, jter->rotation));
}

Matrix4x4f Bone::GetScaling(float time) const {
    if (scalings.size() == 1) {
        return Scale(scalings.front().scaling);
    }

    auto jter = std::upper_bound(scalings.begin(), scalings.end(), KeyScaling(Vector3f::Identity(), time));
    auto iter = jter - 1;
    float t = GetFactor(iter->time, jter->time, time);
    return Scale((1.0f - t) * iter->scaling + t * jter->scaling);
}

Matrix4x4f Bone::GetTransform(float time) const {
    return GetPosition(time) * GetRotation(time) * GetScaling(time);
}
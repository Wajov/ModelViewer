#ifndef BONE_HPP
#define BONE_HPP

#include <string>
#include <vector>

#include <assimp/anim.h>

#include "TypeHelper.hpp"
#include "BoneInfo.hpp"

struct KeyPosition {
    Vector3f position;
    float time;
    KeyPosition(const Vector3f& position, float time) :
        position(position),
        time(time) {}

    ~KeyPosition() {}

    bool operator<(const KeyPosition& position) const {
        return time < position.time;
    }
};

struct KeyRotation {
    Quaternion rotation;
    float time;
    KeyRotation(const Quaternion& rotation, float time) :
        rotation(rotation),
        time(time) {}

    ~KeyRotation() {}

    bool operator<(const KeyRotation& rotation) const {
        return time < rotation.time;
    }
};

struct KeyScaling {
    Vector3f scaling;
    float time;
    KeyScaling(const Vector3f& scaling, float time) :
        scaling(scaling),
        time(time) {}

    ~KeyScaling() {}

    bool operator<(const KeyScaling& scaling) const {
        return time < scaling.time;
    }
};

struct KeyScalingLess {
    bool operator()(const KeyScaling& scaling, float time) {
        return scaling.time < time;
    }

    bool operator()(float time, const KeyScaling& scaling) {
        return time < scaling.time;
    }
};

class Bone {
private:
    BoneInfo info;
    std::vector<KeyPosition> positions;
    std::vector<KeyRotation> rotations;
    std::vector<KeyScaling> scalings;
    float GetFactor(float a, float b, float x) const;
    Matrix4x4f GetPosition(float time) const;
    Matrix4x4f GetRotation(float time) const;
    Matrix4x4f GetScaling(float time) const;

public:
    Bone(const BoneInfo& info, const aiNodeAnim* animation);
    ~Bone();
    BoneInfo GetInfo() const;
    Matrix4x4f GetTransform(float time) const;
};

#endif
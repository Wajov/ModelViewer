#ifndef BONE_INFO_HPP
#define BONE_INFO_HPP

#include "TypeHelper.hpp"

class BoneInfo {
public:
    int index;
    Matrix4x4f offset;
    BoneInfo();
    ~BoneInfo();
};

#endif
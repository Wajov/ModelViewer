#include "BoneInfo.hpp"

BoneInfo::BoneInfo() :
    index(-1),
    offset(Matrix4x4f::Identity()) {}

BoneInfo::~BoneInfo() {}
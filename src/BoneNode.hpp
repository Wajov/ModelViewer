#ifndef BONE_NODE_HPP
#define BONE_NODE_HPP

#include <string>
#include <vector>

#include <assimp/matrix4x4.h>

#include "TypeHelper.hpp"

class BoneNode {
private:
    std::string name;
    Matrix4x4f transform;
    std::vector<BoneNode*> children;

public:
    BoneNode(const std::string& name, const aiMatrix4x4& transform);
    ~BoneNode();
    std::string GetName() const;
    Matrix4x4f GetTransform() const;
    const std::vector<BoneNode*>& GetChildren() const;
    void AddChild(const BoneNode* child);
};

#endif
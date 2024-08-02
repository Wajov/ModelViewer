#include "BoneNode.hpp"

BoneNode::BoneNode(const std::string& name, const aiMatrix4x4& transform) :
    name(name) {
    this->transform << transform.a1, transform.a2, transform.a3, transform.a4,
        transform.b1, transform.b2, transform.b3, transform.b4,
        transform.c1, transform.c2, transform.c3, transform.c4,
        transform.d1, transform.d2, transform.d3, transform.d4;
}

BoneNode::~BoneNode() {
    for (const BoneNode* child : children) {
        delete child;
    }
}

std::string BoneNode::GetName() const {
    return name;
}

Matrix4x4f BoneNode::GetTransform() const {
    return transform;
}

const std::vector<BoneNode*>& BoneNode::GetChildren() const {
    return children;
}

void BoneNode::AddChild(const BoneNode* child) {
    children.push_back(const_cast<BoneNode*>(child));
}
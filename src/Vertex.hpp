#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "TypeHelper.hpp"

class Vertex {
public:
    static constexpr int MAX_BONE_INFLUENCE = 4;
    Vector3f position, normal, tangent, bitangent;
    Vector2f uv;
    int boneIds[MAX_BONE_INFLUENCE];
    float boneWeights[MAX_BONE_INFLUENCE];
    Vertex(const Vector3f& position, const Vector3f& normal);
    Vertex(const Vector3f& position, const Vector3f& normal, const Vector3f& tangent, const Vector3f& bitangent,
        const Vector2f& uv);
    ~Vertex();
};

#endif

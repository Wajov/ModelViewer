#include "Vertex.hpp"

Vertex::Vertex(const Vector3f& position, const Vector3f& normal) :
    position(position),
    normal(normal),
    boneIds{-1, -1, -1, -1},
    boneWeights{0.0f, 0.0f, 0.0f, 0.0f} {}

Vertex::Vertex(const Vector3f& position, const Vector3f& normal, const Vector3f& tangent, const Vector3f& bitangent,
    const Vector2f& uv) :
    position(position),
    normal(normal),
    tangent(tangent),
    bitangent(bitangent) ,
    uv(uv),
    boneIds{-1, -1, -1, -1},
    boneWeights{0.0f, 0.0f, 0.0f, 0.0f} {}

Vertex::~Vertex() {}
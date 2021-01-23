#include "Vertex.h"

Vertex::Vertex(QVector3D &position, QVector3D &normal) {
    this->position = position;
    this->normal = normal;
}

Vertex::Vertex(QVector3D &position, QVector3D &normal, QVector3D &tangent, QVector3D &bitangent, QVector2D &uv) {
    this->position = position;
    this->normal = normal;
    this->tangent = tangent;
    this->bitangent = bitangent;
    this->uv = uv;
}

Vertex::~Vertex() {}
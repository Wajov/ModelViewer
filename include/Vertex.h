#ifndef VERTEX_H
#define VERTEX_H

#include <QVector2D>
#include <QVector3D>

class Vertex {
public:
    QVector3D position, normal, tangent, bitangent;
    QVector2D uv;
    Vertex(QVector3D &position, QVector3D &normal);
    Vertex(QVector3D &position, QVector3D &normal, QVector3D &tangent, QVector3D &bitangent, QVector2D &uv);
    ~Vertex();
};

#endif
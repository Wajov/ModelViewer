#ifndef MESH_H
#define MESH_H

#include <algorithm>
#include <cfloat>
#include <vector>

#include <QVector3D>
#include <QImage>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

#include "Vertex.h"

class Mesh : protected QOpenGLFunctions {
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    QVector3D ambientColor, diffuseColor, specularColor;
    float shininess;
    QImage ambientImage, diffuseImage, specularImage, normalImage;
    QOpenGLTexture *ambientTexture, *diffuseTexture, *specularTexture, *normalTexture;
    QOpenGLVertexArrayObject *vao;
    QOpenGLBuffer vbo, ebo;
    void processTexture(QOpenGLTexture *texture);

public:
    Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, QVector3D &ambientColor, QVector3D &diffuseColor, QVector3D &specularColor, float shininess, QImage &ambientImage, QImage &diffuseImage, QImage &specularImage, QImage &normalImage);
    ~Mesh();
    void coordinateRange(float &xMin, float &xMax, float &yMin, float &yMax, float &zMin, float &zMax);
    void recenter(QVector3D &center);
    void bind(QOpenGLShaderProgram &program);
    void render(QOpenGLShaderProgram &program);
};

#endif
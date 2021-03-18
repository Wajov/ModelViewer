#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, QVector3D &ambientColor, QVector3D &diffuseColor, QVector3D &specularColor, float shininess, QImage &ambientImage, QImage &diffuseImage, QImage &specularImage, QImage &normalImage) {
    this->vertices = vertices;
    this->indices = indices;
    this->ambientColor = ambientColor;
    this->diffuseColor = diffuseColor;
    this->specularColor = specularColor;
    this->shininess = shininess;
    this->ambientImage = ambientImage;
    this->diffuseImage = diffuseImage;
    this->specularImage = specularImage;
    this->normalImage = normalImage;
    vao = nullptr;
    ambientTexture = diffuseTexture = specularTexture = normalTexture = nullptr;
}

Mesh::~Mesh() {
    delete vao;
    delete ambientTexture;
    delete diffuseTexture;
    delete specularTexture;
    delete normalTexture;
}

void Mesh::processTexture(QOpenGLTexture *texture) {
    texture->setWrapMode(QOpenGLTexture::Repeat);
    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->generateMipMaps();
}

void Mesh::coordinateRange(float &xMin, float &xMax, float &yMin, float &yMax, float &zMin, float &zMax) {
    xMin = yMin = zMin = FLT_MAX;
    xMax = yMax = zMax = -FLT_MAX;
    for (Vertex &vertex : vertices) {
        xMin = std::min(xMin, vertex.position.x());
        xMax = std::max(xMax, vertex.position.x());
        yMin = std::min(yMin, vertex.position.y());
        yMax = std::max(yMax, vertex.position.y());
        zMin = std::min(zMin, vertex.position.z());
        zMax = std::max(zMax, vertex.position.z());
    }
}

void Mesh::recenter(QVector3D &center) {
    for (Vertex &vertex : vertices)
        vertex.position -= center;
}

void Mesh::bind(QOpenGLShaderProgram &program) {
    initializeOpenGLFunctions();

    vao = new QOpenGLVertexArrayObject();
    QOpenGLVertexArrayObject::Binder binder(vao);

    vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo.create();
    vbo.bind();
    vbo.allocate(&(vertices[0]), vertices.size() * sizeof(Vertex));

    ebo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    ebo.create();
    ebo.bind();
    ebo.allocate(&(indices[0]), indices.size() * sizeof(unsigned int));

    int attributePosition = program.attributeLocation("position");
    program.setAttributeBuffer(attributePosition, GL_FLOAT, offsetof(Vertex, position), 3, sizeof(Vertex));
    program.enableAttributeArray(attributePosition);

    int attributeNormal = program.attributeLocation("normal");
    program.setAttributeBuffer(attributeNormal, GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));
    program.enableAttributeArray(attributeNormal);

    int attributeTangent = program.attributeLocation("tangent");
    program.setAttributeBuffer(attributeTangent, GL_FLOAT, offsetof(Vertex, tangent), 3, sizeof(Vertex));
    program.enableAttributeArray(attributeTangent);

    int attributeBitanget = program.attributeLocation("bitangent");
    program.setAttributeBuffer(attributeBitanget, GL_FLOAT, offsetof(Vertex, bitangent), 3, sizeof(Vertex));
    program.enableAttributeArray(attributeBitanget);

    int attributeUV = program.attributeLocation("uv");
    program.setAttributeBuffer(attributeUV, GL_FLOAT, offsetof(Vertex, uv), 2, sizeof(Vertex));
    program.enableAttributeArray(attributeUV);

    if (!ambientImage.isNull()) {
        ambientTexture = new QOpenGLTexture(ambientImage);
        processTexture(ambientTexture);
    }
    if (!diffuseImage.isNull()) {
        diffuseTexture = new QOpenGLTexture(diffuseImage);
        processTexture(diffuseTexture);
    }
    if (!specularImage.isNull()) {
        specularTexture = new QOpenGLTexture(specularImage);
        processTexture(specularTexture);
    }
    if (!normalImage.isNull()) {
        normalTexture = new QOpenGLTexture(normalImage);
        processTexture(normalTexture);
    }
}

void Mesh::render(QOpenGLShaderProgram &program) {
    program.setUniformValue("ambientColor", ambientColor);
    program.setUniformValue("diffuseColor", diffuseColor);
    program.setUniformValue("specularColor", specularColor);
    program.setUniformValue("shininess", shininess);

    program.setUniformValue("ambientExist", ambientTexture != nullptr);
    if (ambientTexture != nullptr) {
        program.setUniformValue("ambientTexture", 0);
        glActiveTexture(GL_TEXTURE0);
        ambientTexture->bind();
    }

    program.setUniformValue("diffuseExist", diffuseTexture != nullptr);
    if (diffuseTexture != nullptr) {
        program.setUniformValue("diffuseTexture", 1);
        glActiveTexture(GL_TEXTURE1);
        diffuseTexture->bind();
    }

    program.setUniformValue("specularExist", specularTexture != nullptr);
    if (specularTexture != nullptr) {
        program.setUniformValue("specularTexture", 2);
        glActiveTexture(GL_TEXTURE2);
        specularTexture->bind();
    }

    program.setUniformValue("normalExist", normalTexture != nullptr);
    if (normalTexture != nullptr) {
        program.setUniformValue("normalTexture", 3);
        glActiveTexture(GL_TEXTURE3);
        normalTexture->bind();
    }

    QOpenGLVertexArrayObject::Binder binder(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}
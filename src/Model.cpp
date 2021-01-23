#include "Model.h"

Model::Model() {}

Model::Model(const std::string &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Failed to load model:" << std::endl << importer.GetErrorString() << std::endl;
        return;
    }

    std::string directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene, directory);

    float xMin, xMax, yMin, yMax, zMin, zMax;
    xMin = yMin = zMin = FLT_MAX;
    xMax = yMax = zMax = -FLT_MAX;
    for (Mesh &mesh : meshes) {
        float xMinTemp, xMaxTemp, yMinTemp, yMaxTemp, zMinTemp, zMaxTemp;
        mesh.coordinateRange(xMinTemp, xMaxTemp, yMinTemp, yMaxTemp, zMinTemp, zMaxTemp);
        xMin = std::min(xMin, xMinTemp);
        xMax = std::max(xMax, xMaxTemp);
        yMin = std::min(yMin, yMinTemp);
        yMax = std::max(yMax, yMaxTemp);
        zMin = std::min(zMin, zMinTemp);
        zMax = std::max(zMax, zMaxTemp);
    }
    QVector3D center((xMin + xMax) / 2, (yMin + yMax) / 2, (zMin + zMax) / 2);
    for (Mesh &mesh : meshes)
        mesh.recenter(center);
}

Model::~Model() {}

void Model::processNode(aiNode *node, const aiScene *scene, std::string &directory) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene, directory));
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene, directory);
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene, std::string &directory) {
    std::vector<Vertex> vertices;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        QVector3D position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        QVector3D normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0] != nullptr) {
            QVector3D tangent(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            QVector3D bitangent(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
            QVector2D uv(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            vertices.push_back(Vertex(position, normal, tangent, bitangent, uv));
        } else
            vertices.push_back(Vertex(position, normal));
    }

    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            indices.push_back(mesh->mFaces[i].mIndices[j]);

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    QImage ambient = processTexture(material, aiTextureType_AMBIENT, directory);
    QImage diffuse = processTexture(material, aiTextureType_DIFFUSE, directory);
    QImage specular = processTexture(material, aiTextureType_SPECULAR, directory);
    QImage normal = processTexture(material, aiTextureType_HEIGHT, directory);

    return Mesh(vertices, indices, ambient, diffuse, specular, normal);
}

QImage Model::processTexture(aiMaterial *material, aiTextureType type, std::string &directory) {
    if (material->GetTextureCount(type) > 0) {
        aiString nameTemp;
        material->GetTexture(type, 0, &nameTemp);
        std::string name = nameTemp.C_Str();
        return QImage((directory + "/" + name).c_str());
    } else
        return QImage();
}

void Model::bind(QOpenGLShaderProgram &program) {
    for (Mesh &mesh : meshes)
        mesh.bind(program);
}

void Model::render(QOpenGLShaderProgram &program) {
    for (Mesh &mesh : meshes)
        mesh.render(program);
}
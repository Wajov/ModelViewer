#define STB_IMAGE_IMPLEMENTATION

#include "Model.h"

Model::Model(const std::string &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Failed to load model:" << std::endl << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);

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
    glm::vec3 center((xMin + xMax) / 2, (yMin + yMax) / 2, (zMin + zMax) / 2);
    for (Mesh &mesh : meshes) {
        mesh.recenter(center);
        mesh.bind();
    }
}

Model::~Model() {}

void Model::render(Shader &shader) {
    for (Mesh &mesh : meshes)
        mesh.render(shader);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        glm::vec3 position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0] != nullptr) {
            glm::vec3 tangent(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            glm::vec3 bitangent(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
            glm::vec2 uv(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            vertices.push_back(Vertex(position, normal, tangent, bitangent, uv));
        } else
            vertices.push_back(Vertex(position, normal));
    }

    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            indices.push_back(mesh->mFaces[i].mIndices[j]);

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    Texture ambient = processMaterial(material, aiTextureType_AMBIENT);
    Texture diffuse = processMaterial(material, aiTextureType_DIFFUSE);
    Texture specular = processMaterial(material, aiTextureType_SPECULAR);
    Texture normal = processMaterial(material, aiTextureType_HEIGHT);

    return Mesh(vertices, indices, ambient, diffuse, specular, normal);
}

Texture Model::processMaterial(aiMaterial *material, aiTextureType type) {
    if (material->GetTextureCount(type) > 0) {
        aiString nameTemp;
        material->GetTexture(type, 0, &nameTemp);
        std::string name = nameTemp.C_Str();

        for (Texture &texture : textures)
            if (texture.getName() == name)
                return texture;

        Texture texture(processTexture(name), name);
        textures.push_back(texture);
        return texture;
    } else
        return Texture();
}

unsigned int Model::processTexture(std::string &name) {
    std::string path = directory + "/" + name;
    
    unsigned int id;
    glGenTextures(1, &id);

    int width, height, channels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data) {
        GLenum format;
        switch (channels) {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
        }

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else
        std::cerr << "Failed to load texture file: " << path << std::endl;
    
    stbi_image_free(data);
    return id;
}

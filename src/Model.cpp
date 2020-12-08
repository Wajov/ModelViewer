#include "Model.h"

Model::Model(const std::string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Failed to load model:" << std::endl << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);

    float xMin, xMax, yMin, yMax, zMin, zMax;
    xMin = yMin = zMin = 1e38f;
    xMax = yMax = zMax = -1e38f;
    for (unsigned int i = 0; i < meshes.size(); i++) {
        float xMinTemp, xMaxTemp, yMinTemp, yMaxTemp, zMinTemp, zMaxTemp;
        meshes[i].coordinateRange(xMinTemp, xMaxTemp, yMinTemp, yMaxTemp, zMinTemp, zMaxTemp);
        xMin = std::min(xMin, xMinTemp);
        xMax = std::max(xMax, xMaxTemp);
        yMin = std::min(yMin, yMinTemp);
        yMax = std::max(yMax, yMaxTemp);
        zMin = std::min(zMin, zMinTemp);
        zMax = std::max(zMax, zMaxTemp);
    }
    glm::vec3 center((xMin + xMax) / 2, (yMin + yMax) / 2, (zMin + zMax) / 2);
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].recenter(center);
        meshes[i].bind();
    }
}

void Model::render(Shader shader) {
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].render(shader);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );
        vertex.normal = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        );
        if (mesh->mTextureCoords[0])
            vertex.uv = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        else
            vertex.uv = glm::vec2(0.0f, 0.0f);
        vertex.tan = glm::vec3(
            mesh->mTangents[i].x,
            mesh->mTangents[i].y,
            mesh->mTangents[i].z
        );
        vertex.bitan = glm::vec3(
            mesh->mBitangents[i].x,
            mesh->mBitangents[i].y,
            mesh->mBitangents[i].z
        );
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string name) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
        aiString s;
        material->GetTexture(type, i, &s);
        bool flag = false;
        for (unsigned int j = 0; j < this->textures.size(); j++)
            if (std::strcmp(this->textures[j].path.c_str(), s.C_Str()) == 0) {
                textures.push_back(this->textures[j]);
                flag = true;
                break;
            }
        if (!flag) {
            Texture texture;
            texture.id = loadTextureFromFile(s.C_Str(), directory);
            texture.type = name;
            texture.path = s.C_Str();
            textures.push_back(texture);
            this->textures.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::loadTextureFromFile(const char *path, const std::string &directory) {
    std::string filename = directory + "/" + std::string(path);
    
    unsigned int textureId;
    glGenTextures(1, &textureId);

    int width, height, channels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
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

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else
        std::cerr << "Failed to load texture file: " << path << std::endl;
    
    stbi_image_free(data);
    return textureId;
}

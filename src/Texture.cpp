#include "Texture.h"

Texture::Texture() {
    exist = false;
}

Texture::Texture(unsigned int id, const std::string &name) {
    exist = true;
    this->id = id;
    this->name = name;
}

Texture::~Texture() {}

bool Texture::getExist() {
    return exist;
}

unsigned int Texture::getId() {
    return id;
}

std::string Texture::getName() {
    return name;
}
#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include "Image.hpp"

Image::Image(const std::string& path) {
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
}

Image::~Image() {
    stbi_image_free(data);
}

int Image::getWidth() const {
    return width;
}

int Image::getHeight() const {
    return height;
}

int Image::getChannels() const {
    return channels;
}

unsigned char* Image::getData() const {
    return data;
}
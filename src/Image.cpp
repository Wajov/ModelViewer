#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include "Image.hpp"

Image::Image(const std::string& path) {
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
}

Image::~Image() {
    stbi_image_free(data);
}

int Image::GetWidth() const {
    return width;
}

int Image::GetHeight() const {
    return height;
}

int Image::GetChannels() const {
    return channels;
}

unsigned char* Image::GetData() const {
    return data;
}
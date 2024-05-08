#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>

class Image {
private:
    int width, height, channels;
    unsigned char* data;

public:
    Image(const std::string& path);
    ~Image();
    int getWidth() const;
    int getHeight() const;
    int getChannels() const;
    unsigned char* getData() const;
};

#endif
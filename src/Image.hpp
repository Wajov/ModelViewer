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
    int GetWidth() const;
    int GetHeight() const;
    int GetChannels() const;
    unsigned char* GetData() const;
};

#endif
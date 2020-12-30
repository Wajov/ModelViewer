#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture {
private:
    bool exist;
    unsigned int id;
    std::string name;

public:
    Texture();
    Texture(unsigned int id, const std::string &name);
    ~Texture();
    bool getExist();
    unsigned int getId();
    std::string getName();
};

#endif
#ifndef VIEWER_HPP
#define VIEWER_HPP

#include <string>

#include "Model.hpp"
#include "Animation.hpp"
#include "Renderer.hpp"

class Viewer {
private:
    Model* model;
    Animation* animation;
    Renderer* renderer;

public:
    Viewer(const std::string& modelPath, const std::string& animationPath = "");
    ~Viewer();
    void Start();
};

#endif
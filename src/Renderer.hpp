#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cmath>

#include <GLFW/glfw3.h>

#include "TypeHelper.hpp"

class Renderer {
private:
    bool leftPress, rightPress;
    int width, height;
    double leftX, leftY, rightX, rightY, scaling;
    Vector3f translation, lightDirection, cameraPosition;
    Matrix4x4f rotation;
    GLFWwindow* window;
    void FramebufferSizeCallback(int width, int height);
    void MouseButtonCallback(int button, int action, int mods);
    void CursorPosCallback(double x, double y);
    void ScrollCallback(double x, double y);
    void KeyCallback(int key, int scancode, int action, int mods);

public:
    Renderer(int width, int height);
    ~Renderer();
    GLFWwindow* GetWindow() const;
    Vector3f GetLightDirection() const;
    Vector3f GetCameraPosition() const;
    Matrix4x4f GetModel() const;
    Matrix4x4f GetView() const;
    Matrix4x4f GetProjection() const;
};

#endif
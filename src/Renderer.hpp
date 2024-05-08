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
    Matrix4x4f scale(float scaling) const;
    Matrix4x4f rotate(const Vector3f& v, float angle) const;
    Matrix4x4f translate(const Vector3f& v) const;
    Matrix4x4f lookAt(const Vector3f& position, const Vector3f& center, const Vector3f& up) const;
    Matrix4x4f perspective(float fovy, float aspect, float zNear, float zFar) const;
    void framebufferSizeCallback(int width, int height);
    void mouseButtonCallback(int button, int action, int mods);
    void cursorPosCallback(double x, double y);
    void scrollCallback(double x, double y);
    void keyCallback(int key, int scancode, int action, int mods);

public:
    Renderer(int width, int height);
    ~Renderer();
    GLFWwindow* getWindow() const;
    Vector3f getLightDirection() const;
    Vector3f getCameraPosition() const;
    Matrix4x4f getModel() const;
    Matrix4x4f getView() const;
    Matrix4x4f getProjection() const;
};

#endif
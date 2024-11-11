#include <algorithm>
#include <iostream>

#include <glad/glad.h>

#include "Renderer.hpp"
#include "TransformationHelper.hpp"

Renderer::Renderer(int width, int height) :
    width(width),
    height(height),
    leftPress(false),
    rightPress(false),
    leftX(INFINITY),
    leftY(INFINITY),
    rightX(INFINITY),
    rightY(INFINITY),
    scaling(1.0f),
    translation(Vector3f::Zero()),
    rotation(Matrix4x4f::Identity()),
    lightDirection(0.0f, 0.0f, 1.0f),
    cameraPosition(0.0f, 0.0f, 5.0f) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(width, height, "ClothSimulator", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        static_cast<Renderer*>(glfwGetWindowUserPointer(window))->FramebufferSizeCallback(width, height);
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        static_cast<Renderer*>(glfwGetWindowUserPointer(window))->MouseButtonCallback(button, action, mods);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
        static_cast<Renderer*>(glfwGetWindowUserPointer(window))->CursorPosCallback(x, y);
    });
    glfwSetScrollCallback(window, [](GLFWwindow* window, double x, double y) {
        static_cast<Renderer*>(glfwGetWindowUserPointer(window))->ScrollCallback(x, y);
    });
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        static_cast<Renderer*>(glfwGetWindowUserPointer(window))->KeyCallback(key, scancode, action, mods);
    });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
}

Renderer::~Renderer() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Renderer::FramebufferSizeCallback(int width, int height) {
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);
}

void Renderer::MouseButtonCallback(int button, int action, int mods) {
    double x, y;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        leftPress = true;
        leftX = leftY = INFINITY;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        rightPress = true;
        rightX = rightY = INFINITY;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        rightPress = false;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        leftPress = false;
    }
}

void Renderer::CursorPosCallback(double x, double y) {
    if (leftPress && leftX != INFINITY && leftY != INFINITY) {
        Vector3f a = (Vector3f(static_cast<float>(leftX) / width - 0.5f, 0.5f - static_cast<float>(leftY) / height,
            1.0f)).normalized();
        Vector3f b = (Vector3f(static_cast<float>(x) / width - 0.5f, 0.5f - static_cast<float>(y) / height,
            1.0f)).normalized();
        Vector3f axis = a.cross(b);
        float angle = a.dot(b);
        rotation = Rotate(axis, 10.0f * acos(angle)) * rotation;
    }
    if (rightPress && rightX != INFINITY && rightY != INFINITY) {
        Vector3f v(static_cast<float>(x) - rightX, rightY - static_cast<float>(y), 0.0f);
        translation += 0.005f * v;
    }

    leftX = rightX = x;
    leftY = rightY = y;
}

void Renderer::ScrollCallback(double x, double y) {
    scaling += 0.1 * y;
    scaling = std::max(scaling, 0.01);
}

void Renderer::KeyCallback(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWwindow* Renderer::GetWindow() const {
    return window;
}

Vector3f Renderer::GetLightDirection() const {
    return lightDirection;
}

Vector3f Renderer::GetCameraPosition() const {
    return cameraPosition;
}

Matrix4x4f Renderer::GetModel() const {
    return Translate(translation) * Scale(static_cast<float>(scaling)) * rotation;
}

Matrix4x4f Renderer::GetView() const {
    return LookAt(cameraPosition, Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f));
}

Matrix4x4f Renderer::GetProjection() const {
    return Perspective(0.5f * M_PI, static_cast<float>(width) / height, 0.1f, 100.0f);
}
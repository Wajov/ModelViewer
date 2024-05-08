#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "TypeHelper.hpp"
#include "Model.hpp"
#include "Renderer.hpp"

int main(int argc, char **argv) {
    Renderer* renderer = new Renderer(1920, 1080);
    Model* model = new Model(argv[1]);
    model->bind();

    while (!glfwWindowShouldClose(renderer->getWindow())) {
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Vector3f lightDirection = renderer->getLightDirection();
        Vector3f cameraPosition = renderer->getCameraPosition();
        Matrix4x4f modelMat = renderer->getModel();
        Matrix4x4f viewMat = renderer->getView();
        Matrix4x4f projectionMat = renderer->getProjection();
        model->render(modelMat, viewMat, projectionMat, cameraPosition, lightDirection);

        glfwSwapBuffers(renderer->getWindow());
        glfwPollEvents();
    }

    delete model;
    delete renderer;
    return 0;
}
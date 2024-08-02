#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Viewer.hpp"

Viewer::Viewer(const std::string& modelPath, const std::string& animationPath) {
    model = new Model(modelPath);
    animation = animationPath.empty() ? nullptr : new Animation(animationPath, model);
    renderer = new Renderer(1920, 1080);
}

Viewer::~Viewer() {
    delete model;
    if (animation != nullptr) {
        delete animation;
    }
    delete renderer;
}

void Viewer::Start() {
    model->Bind();

    while (!glfwWindowShouldClose(renderer->GetWindow())) {
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (animation != nullptr) {
            std::vector<Matrix4x4f> finalBoneMatrices = animation->GetFinalBoneMatrices(glfwGetTime());
            model->SetFinalBoneMatrices(finalBoneMatrices);
        }

        Vector3f lightDirection = renderer->GetLightDirection();
        Vector3f cameraPosition = renderer->GetCameraPosition();
        Matrix4x4f modelMat = renderer->GetModel();
        Matrix4x4f viewMat = renderer->GetView();
        Matrix4x4f projectionMat = renderer->GetProjection();
        model->SetAnimationExist(animation != nullptr);
        model->Render(modelMat, viewMat, projectionMat, cameraPosition, lightDirection);

        glfwSwapBuffers(renderer->GetWindow());
        glfwPollEvents();
    }
}
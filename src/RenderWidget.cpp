#include "RenderWidget.h"

RenderWidget::RenderWidget() : QOpenGLWidget() {
    press = false;
    factor = 1.0f;
    model = new Model();
}

RenderWidget::~RenderWidget() {}

void RenderWidget::initializeGL() {
    initializeOpenGLFunctions();

    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "shader/VertexShader.glsl")) {
        std::cerr << "Failed to add vertex shader" << std::endl;
        return;
    }
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, "shader/FragmentShader.glsl")) {
        std::cerr << "Failed to add fragment shader" << std::endl;
        return;
    }
    if (!program.link()) {
        std::cerr << "Failed to link shader program" << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);
    model->bind(program);
}

void RenderWidget::paintGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.bind();

    QVector3D lightDirection(0.0f, 0.0f, -1.0f), cameraPosition(0.0f, 0.0f, 5.0f);
    QMatrix4x4 modelMat, viewMat, projectionMat;
    modelMat = rotate;
    modelMat.scale(factor);
    viewMat.lookAt(cameraPosition, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
    projectionMat.perspective(45.0f, (float)width() / (float)height(), 0.1f, 100.0f);

    program.setUniformValue("model", modelMat);
    program.setUniformValue("view", viewMat);
    program.setUniformValue("projection", projectionMat);
    program.setUniformValue("lightDirection", lightDirection);
    program.setUniformValue("cameraPosition", cameraPosition);
    model->render(program);

    program.release();
}

void RenderWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void RenderWidget::mousePressEvent(QMouseEvent *event) {
    press = true;
    lastX = lastY = INT_MIN;
}

void RenderWidget::mouseReleaseEvent(QMouseEvent *event) {
    press = false;
}

void RenderWidget::mouseMoveEvent(QMouseEvent *event) {
    if (press && lastX != INT_MIN && lastY != INT_MIN && (event->x() != lastX || event->y() != lastY)) {
        QVector3D a = QVector3D((float)lastX / (float)width() - 0.5f, 0.5f - (float)lastY / (float)height(), 1.0f).normalized();
        QVector3D b = QVector3D((float)event->x() / (float)width() - 0.5f, 0.5f - (float)event->y() / (float)height(), 1.0f).normalized();
        QVector3D axis = QVector3D::crossProduct(a, b);
        float angle = std::acos(QVector3D::dotProduct(a, b));
        QMatrix4x4 temp;
        temp.rotate(angle / (float)M_PI * 1800.0f, axis);
        rotate = temp * rotate;
    }

    lastX = event->x();
    lastY = event->y();
    update();
}

void RenderWidget::wheelEvent(QWheelEvent *event) {
    factor += (float)event->angleDelta().y() * 0.01f;
    factor = std::max(factor, 0.01f);
    update();
}

void RenderWidget::setModel(const std::string &path) {
    delete model;
    model = new Model(path);
    model->bind(program);
    update();
}
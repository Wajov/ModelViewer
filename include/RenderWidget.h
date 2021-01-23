#ifndef RENDER_WIDGET_H
#define RENDER_WIDGET_H

#include <climits>
#include <iostream>
#include <string>

#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <QWheelEvent>

#include "Model.h"

class RenderWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
private:
    bool press;
    int lastX, lastY;
    float factor;
    QMatrix4x4 rotate;
    Model *model;
    QOpenGLShaderProgram program;
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

public:
    RenderWidget();
    ~RenderWidget() override;
    void setModel(const std::string &path);
};

#endif
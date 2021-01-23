#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <string>

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QStringList>

#include "Model.h"
#include "RenderWidget.h"

class RenderWindow : public QMainWindow {
    Q_OBJECT
private:
    RenderWidget *widget;
    QMenuBar *bar;
    QMenu *menu;
    QAction *open, *exit;

public:
    RenderWindow(int width, int height);
    ~RenderWindow() override;

private slots:
    void selectModel();
};

#endif
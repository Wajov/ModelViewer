#include "RenderWindow.h"

RenderWindow::RenderWindow(int width, int height) : QMainWindow() {
    widget = new RenderWidget(this, "");
    setCentralWidget(widget);
    resize(width, height);

    bar = menuBar();
    menu = bar->addMenu("文件");
    open = menu->addAction("打开");
    connect(open, SIGNAL(triggered(bool)), this, SLOT(selectModel()));
    exit = menu->addAction("退出");
    connect(exit, SIGNAL(triggered(bool)), this, SLOT(close()));
}

RenderWindow::~RenderWindow() {}

void RenderWindow::selectModel() {
    QString path = QFileDialog::getOpenFileName(this,"打开",".","");
    std::cout << path.toStdString() << std::endl;
    delete widget;
    widget = new RenderWidget(this, path.toStdString());
    widget->setParent(this);
    setCentralWidget(widget);
}
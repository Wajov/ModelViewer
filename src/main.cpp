#include <QApplication>

#include "RenderWindow.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    RenderWindow window(1600, 900);
    window.show();

    return QApplication::exec();
}
#include <iostream>

#include "Viewer.hpp"

int main(int argc, char** argv) {
    Viewer* viewer;
    if (argc == 2) {
        viewer = new Viewer(argv[1]);
        std::cout << "Without animation" << std::endl;
    } else if (argc == 3) {
        viewer = new Viewer(argv[1], argv[2]);
        std::cout << "With animation" << std::endl;
    } else {
        std::cerr << "Expected 1 or 2 arguments" << std::endl;
        exit(1);
    }
    viewer->Start();

    delete viewer;
    return 0;
}
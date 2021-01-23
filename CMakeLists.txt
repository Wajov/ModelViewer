cmake_minimum_required(VERSION 3.0)
project(ModelViewer)

set(CMAKE_AUTOMOC ON)

include_directories(include)
set(HEADER
        include/Mesh.h
        include/Model.h
        include/RenderWidget.h
        include/Vertex.h
        include/RenderWindow.h)
aux_source_directory(src SOURCE)
add_executable(ModelViewer
        ${HEADER}
        ${SOURCE}
)

# Assimp
find_package(assimp REQUIRED)

# Qt
set(QT_VERSION 5)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
target_link_libraries(
        ModelViewer
        assimp
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)
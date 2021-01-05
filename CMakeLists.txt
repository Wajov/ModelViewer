cmake_minimum_required(VERSION 3.0)
project(ModelViewer)

include_directories(
    include
    lib/glad/include
)
aux_source_directory(src SOURCE)
add_executable(ModelViewer ${SOURCE})

# OpenGL
find_package(OpenGL REQUIRED)

# GLAD
add_subdirectory(lib/glad)

# GLFW
find_package(glfw3 REQUIRED)

# Assimp
find_package(assimp REQUIRED)

set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
target_link_libraries(
    ModelViewer
    glad
    glfw
    assimp
)
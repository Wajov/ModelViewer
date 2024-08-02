cmake_minimum_required(VERSION 3.20)

if (WIN32)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    add_compile_definitions(_USE_MATH_DEFINES)
endif()

project(ModelViewer)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
file(GLOB_RECURSE SOURCE "src/*.cpp" "src/*.hpp")
add_executable(ModelViewer ${SOURCE})

# OpenGL
find_package(OpenGL REQUIRED)

# GLAD
add_subdirectory(lib/glad)

# GLFW
find_package(glfw3 REQUIRED)

# Eigen
find_package(Eigen3 REQUIRED)

# stb
find_package(Stb REQUIRED)

# Assimp
find_package(assimp REQUIRED)

set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
target_link_libraries(
        ModelViewer
        glad
        glfw
        assimp::assimp
)
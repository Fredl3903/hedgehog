cmake_minimum_required(VERSION 3.10)
project(hedgehog)

set(GLFW_ROOT "C:/OpenGL/GLFW")
set(GLEW_ROOT "C:/OpenGL/GLEW")
set(JSON_ROOT "C:OpenGL/nlohmannJSON")

# Define GLEW_STATIC to ensure static linking of GLEW
add_definitions(-DGLEW_STATIC)


# Include directories
include_directories(${GLFW_ROOT}/include/GLFW ${GLEW_ROOT}/include/GL ${JSON_ROOT}/include)
# Link directories
link_directories(${GLFW_ROOT}/lib-mingw-w64 ${GLEW_ROOT}/lib) # link JSON library later

# Find OpenGL package
find_package(OpenGL REQUIRED)


# Define executable target
add_executable(hedgehog src/main.cpp)

# Link libraries to target "hedgehog"
target_link_libraries(hedgehog glfw3 glew32s OpenGL::GL)

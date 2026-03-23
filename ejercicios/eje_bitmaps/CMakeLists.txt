cmake_minimum_required(VERSION 4.1)
project(UAM_eje_bitmaps)

set(CMAKE_CXX_STANDARD 26)

# buscamos las librerias de OpenGl y GLUT para ubuntu 24
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# agregamos el archivo principal del proyecto
add_executable(UAM_eje_bitmaps main.cpp)

# Conectamos tu ejecutable con las librerías gráficas
target_link_libraries(UAM_eje_bitmaps
        OpenGL::GL
        OpenGL::GLU
        GLUT::GLUT
)

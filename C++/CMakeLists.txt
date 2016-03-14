cmake_minimum_required(VERSION 2.6)
file(TO_CMAKE_PATH "$ENV{IMAGINEPP_ROOT}" d)
if(NOT EXISTS "${d}")
    message(FATAL_ERROR "Error: environment variable IMAGINEPP_ROOT=" "${d}")
endif()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${d}/CMake")
find_package(Imagine)

project(Flou)
add_executable(Flou
        main.cpp
        codebarre/codebarre.cpp
        codebarre/codebarre.h
        gradient/gradient.cpp
        gradient/gradient.h)
ImagineUseModules(Flou Graphics)

add_executable(Generateur
        generateur.cpp
        codebarre/codebarre.cpp)
ImagineUseModules(Generateur Graphics)
cmake_minimum_required(VERSION 3.10)
project(MSimplificationAndReM)

set(CMAKE_CXX_STANDARD 14)

add_executable(MeshProcess main.cpp Face.cpp Matrix.cpp MeshSim.cpp Vector.cpp)

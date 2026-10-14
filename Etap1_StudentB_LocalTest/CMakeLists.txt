cmake_minimum_required(VERSION 3.20)
project(StudentB_LocalTest LANGUAGES CXX)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../StudentB_PolynomialLib PolynomialLib_build)

add_executable(StudentB_LocalTest main.cpp)
target_link_libraries(StudentB_LocalTest PRIVATE PolynomialLib)

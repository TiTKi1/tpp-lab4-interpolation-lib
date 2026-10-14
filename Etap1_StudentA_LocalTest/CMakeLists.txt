cmake_minimum_required(VERSION 3.20)
project(StudentA_LocalTest LANGUAGES CXX)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../StudentA_InterpolationLib InterpolationLib_build)

add_executable(StudentA_LocalTest main.cpp)
target_link_libraries(StudentA_LocalTest PRIVATE InterpolationLib)

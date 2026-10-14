cmake_minimum_required(VERSION 3.20)
project(Etap2_SDK_Build LANGUAGES CXX)

# =======================================================================
# Збирає ДИНАМІЧНУ (SHARED) версію власної бібліотеки InterpolationLib
# (Студент А, Варіант 3). POST_BUILD крок у
# StudentA_InterpolationLib/CMakeLists.txt одразу пакує SDK
# (README + include + lib + bin) у SDK_InterpolationLib/ поруч із цим
# каталогом - саме цей пакунок передається партнеру (Студенту Б).
# =======================================================================

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(INTERPOLATIONLIB_BUILD_SHARED ON CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../StudentA_InterpolationLib InterpolationLib_shared_build)

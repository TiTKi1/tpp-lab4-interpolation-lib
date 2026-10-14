cmake_minimum_required(VERSION 3.20)
project(Etap2_SDK_Build LANGUAGES CXX)

# =======================================================================
# Цей проєкт збирає DИНАМІЧНІ (SHARED) версії обох бібліотек. Кожна з
# них сама пакує свій SDK (README + include + lib + bin) у POST_BUILD
# кроці - див. StudentB_PolynomialLib/CMakeLists.txt та
# PartnerLib_InterpolationMock/CMakeLists.txt.
# Результат: SDK_PolynomialLib/ та SDK_InterpolationMock/ поруч із цим
# каталогом, готові до передачі партнеру / використання в Етапі 3.
# =======================================================================

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# --- Власна бібліотека Студента Б: PolynomialLib (SHARED) ---
set(POLYNOMIALLIB_BUILD_SHARED ON CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../StudentB_PolynomialLib PolynomialLib_shared_build)

# --- Mock-бібліотека партнера: InterpolationLib (SHARED) ---
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../PartnerLib_InterpolationMock InterpolationLib_build)

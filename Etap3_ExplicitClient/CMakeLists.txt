cmake_minimum_required(VERSION 3.20)
project(Etap3_ExplicitClient LANGUAGES CXX)

# Явне зв'язування з РЕАЛЬНИМ SDK партнера: НЕ використовуємо .h та .lib,
# лише готовий .dll, завантажений через LoadLibrary/GetProcAddress.

set(PARTNER_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../PolyUtils_SDK)

if(NOT EXISTS "${PARTNER_SDK_DIR}/bin/PolyUtils.dll")
    message(FATAL_ERROR "PolyUtils_SDK not found at ${PARTNER_SDK_DIR}.")
endif()

add_executable(ExplicitClient main.cpp)

add_custom_command(TARGET ExplicitClient POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PARTNER_SDK_DIR}/bin/PolyUtils.dll"
        "$<TARGET_FILE_DIR:ExplicitClient>"
)

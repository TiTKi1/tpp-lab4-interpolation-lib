cmake_minimum_required(VERSION 3.20)
project(InterpolationLib LANGUAGES CXX)

# Ця бібліотека завжди збирається як SHARED (DLL): вона потрібна лише
# для формування SDK-пакунка, який "ніби отримано від партнера" (Етап 3).
add_library(InterpolationLib SHARED src/InterpolationLib.cpp)

target_compile_definitions(InterpolationLib
    PUBLIC INTERPOLATIONLIB_SHARED
    PRIVATE INTERPOLATIONLIB_EXPORTS
)

# Уніфікуємо назви файлів між MSVC та MinGW: InterpolationLib.dll / .lib
set_target_properties(InterpolationLib PROPERTIES
    PREFIX ""
    IMPORT_PREFIX ""
    IMPORT_SUFFIX ".lib"
)

target_include_directories(InterpolationLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(InterpolationLib PUBLIC cxx_std_20)

# Пакуємо готовий SDK (README + include + lib + bin) у SDK_InterpolationMock/ -
# "ніби отриманий від партнера" пакунок, що використовується в Етапі 3.
set(INTERP_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../SDK_InterpolationMock)
add_custom_command(TARGET InterpolationLib POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${INTERP_SDK_DIR}/include"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${INTERP_SDK_DIR}/lib"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${INTERP_SDK_DIR}/bin"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/include/InterpolationLib.h"
        "${INTERP_SDK_DIR}/include/InterpolationLib.h"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/README_CONTRACT.md"
        "${INTERP_SDK_DIR}/README.md"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:InterpolationLib>"
        "${INTERP_SDK_DIR}/bin/$<TARGET_FILE_NAME:InterpolationLib>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_LINKER_FILE:InterpolationLib>"
        "${INTERP_SDK_DIR}/lib/$<TARGET_LINKER_FILE_NAME:InterpolationLib>"
    COMMENT "Packing SDK_InterpolationMock (include/lib/bin + README.md) - stand-in for partner's real SDK"
)

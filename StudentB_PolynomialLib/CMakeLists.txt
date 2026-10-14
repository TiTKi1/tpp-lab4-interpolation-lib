cmake_minimum_required(VERSION 3.20)
project(PolynomialLib LANGUAGES CXX)

# BUILD_SHARED визначається з коду, який підключає цю підпапку:
#   add_subdirectory(StudentB_PolynomialLib) -> STATIC (Етап 1)
# Коли потрібна динамічна версія (Етап 2), використовується окремий
# каталог збірки з -DPOLYNOMIALLIB_BUILD_SHARED=ON (див. корінний CMakeLists.txt).

option(POLYNOMIALLIB_BUILD_SHARED "Build PolynomialLib as a SHARED (DLL) library" OFF)

if(POLYNOMIALLIB_BUILD_SHARED)
    add_library(PolynomialLib SHARED src/PolynomialLib.cpp)
    target_compile_definitions(PolynomialLib
        PUBLIC POLYNOMIALLIB_SHARED
        PRIVATE POLYNOMIALLIB_EXPORTS
    )
    # Уніфікуємо назви файлів між MSVC та MinGW: PolynomialLib.dll / PolynomialLib.lib
    # (без префіксу "lib", з розширенням .lib для імпортної бібліотеки), щоб
    # SDK-пакунок виглядав однаково незалежно від компілятора.
    set_target_properties(PolynomialLib PROPERTIES
        PREFIX ""
        IMPORT_PREFIX ""
        IMPORT_SUFFIX ".lib"
    )
else()
    add_library(PolynomialLib STATIC src/PolynomialLib.cpp)
endif()

target_include_directories(PolynomialLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(PolynomialLib PUBLIC cxx_std_20)

# Коли бібліотека збирається як SHARED (Етап 2), одразу пакуємо
# готовий SDK (README + include + lib + bin) у SDK_PolynomialLib/,
# який передається партнеру і використовується в Етапі 3.
if(POLYNOMIALLIB_BUILD_SHARED)
    set(POLY_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../SDK_PolynomialLib)
    add_custom_command(TARGET PolynomialLib POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${POLY_SDK_DIR}/include"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${POLY_SDK_DIR}/lib"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${POLY_SDK_DIR}/bin"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/include/PolynomialLib.h"
            "${POLY_SDK_DIR}/include/PolynomialLib.h"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/README_CONTRACT.md"
            "${POLY_SDK_DIR}/README.md"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:PolynomialLib>"
            "${POLY_SDK_DIR}/bin/$<TARGET_FILE_NAME:PolynomialLib>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_LINKER_FILE:PolynomialLib>"
            "${POLY_SDK_DIR}/lib/$<TARGET_LINKER_FILE_NAME:PolynomialLib>"
        COMMENT "Packing SDK_PolynomialLib (include/lib/bin + README.md)"
    )
endif()

cmake_minimum_required(VERSION 3.20)
project(Etap3_ImplicitClient LANGUAGES CXX)

# Неявне зв'язування з РЕАЛЬНИМ SDK партнера (PolyUtils, отриманий файлом
# від напарника, а не mock). Підключаємо .h для компіляції і .lib для
# лінкування.

set(PARTNER_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../PolyUtils_SDK)

if(NOT EXISTS "${PARTNER_SDK_DIR}/lib/PolyUtils.lib")
    message(FATAL_ERROR "PolyUtils_SDK not found at ${PARTNER_SDK_DIR}.")
endif()

add_executable(ImplicitClient main.cpp)

target_include_directories(ImplicitClient PRIVATE
    ${PARTNER_SDK_DIR}/include
)

target_link_libraries(ImplicitClient PRIVATE
    ${PARTNER_SDK_DIR}/lib/PolyUtils.lib
)

# Копіюємо DLL поруч з .exe для неявного (load-time) зв'язування.
add_custom_command(TARGET ImplicitClient POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PARTNER_SDK_DIR}/bin/PolyUtils.dll"
        "$<TARGET_FILE_DIR:ImplicitClient>"
)

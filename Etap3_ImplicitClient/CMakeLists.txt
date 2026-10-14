cmake_minimum_required(VERSION 3.20)
project(Etap3_ImplicitClient LANGUAGES CXX)

# Неявне зв'язування: підключаємо .h для компіляції і .lib для лінкування.
# Жодного коду з CMakeLists.txt партнера ми не чіпаємо - лише готовий SDK.

set(PARTNER_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../SDK_InterpolationMock)

if(NOT EXISTS "${PARTNER_SDK_DIR}/lib/InterpolationLib.lib")
    message(FATAL_ERROR
        "SDK_InterpolationMock not found at ${PARTNER_SDK_DIR}. "
        "Build Etap2_SDK_Build first (it packages the SDK this client links against).")
endif()

add_executable(ImplicitClient main.cpp)

target_include_directories(ImplicitClient PRIVATE
    ${PARTNER_SDK_DIR}/include
)

target_link_libraries(ImplicitClient PRIVATE
    ${PARTNER_SDK_DIR}/lib/InterpolationLib.lib
)

# Копіюємо DLL поруч з .exe, щоб неявне зв'язування могло завантажити
# бібліотеку під час запуску (load-time linking).
add_custom_command(TARGET ImplicitClient POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PARTNER_SDK_DIR}/bin/InterpolationLib.dll"
        "$<TARGET_FILE_DIR:ImplicitClient>"
)

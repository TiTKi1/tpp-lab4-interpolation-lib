cmake_minimum_required(VERSION 3.20)
project(Etap3_ExplicitClient LANGUAGES CXX)

# Явне зв'язування: НЕ використовуємо .h та .lib партнера взагалі.
# Єдина залежність під час компіляції - windows.h (LoadLibrary/GetProcAddress).

set(PARTNER_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../SDK_InterpolationMock)

if(NOT EXISTS "${PARTNER_SDK_DIR}/bin/InterpolationLib.dll")
    message(FATAL_ERROR
        "SDK_InterpolationMock not found at ${PARTNER_SDK_DIR}. "
        "Build Etap2_SDK_Build first (it packages the SDK this client uses).")
endif()

add_executable(ExplicitClient main.cpp)

# Копіюємо тільки .dll (без .h і .lib) поруч з .exe - саме так, як і має
# виглядати явне зв'язування: програма нічого не знає про бібліотеку
# на етапі компонування, лише завантажує готовий .dll під час виконання.
add_custom_command(TARGET ExplicitClient POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PARTNER_SDK_DIR}/bin/InterpolationLib.dll"
        "$<TARGET_FILE_DIR:ExplicitClient>"
)

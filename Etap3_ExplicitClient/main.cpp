#include <iostream>
#include <windows.h> // LoadLibrary, GetProcAddress, FreeLibrary

// Сигнатури функцій, які ми очікуємо знайти в InterpolationLib.dll.
// Жодного .h файлу партнера тут немає - лише ці оголошення типів покажчиків,
// складені власноруч за документацією (README.md / контрактом) партнера.
typedef double (*LinearInterpolationFunc)(double, double, double, double, double);
typedef double (*LagrangeFunc)(const double*, const double*, int, double);
typedef double (*InterpolateFunc)(const double*, const double*, int, double);
typedef double (*InterpolationErrorFunc)(const double*, const double*, int, double, double);

int main()
{
    std::cout << "=== Explicit linking: InterpolationLib (partner SDK, DLL only) ===" << std::endl;

    HMODULE library = LoadLibrary(TEXT("InterpolationLib.dll"));
    if (library == nullptr)
    {
        std::cerr << "ERROR: Could not load InterpolationLib.dll!" << std::endl;
        return 1;
    }
    std::cout << "DLL is successfully loaded!" << std::endl;

    auto linearInterpolation = reinterpret_cast<LinearInterpolationFunc>(
        GetProcAddress(library, "linearInterpolation"));
    auto lagrange = reinterpret_cast<LagrangeFunc>(
        GetProcAddress(library, "lagrange"));
    auto interpolate = reinterpret_cast<InterpolateFunc>(
        GetProcAddress(library, "interpolate"));
    auto interpolationError = reinterpret_cast<InterpolationErrorFunc>(
        GetProcAddress(library, "interpolationError"));

    if (!linearInterpolation || !lagrange || !interpolate || !interpolationError)
    {
        std::cerr << "ERROR: one or more functions were not found in the DLL "
                   << "(check for C++ name mangling / extern \"C\")!" << std::endl;
        FreeLibrary(library);
        return 1;
    }
    std::cout << "All functions were found via GetProcAddress." << std::endl;

    double xs[] = { 0.0, 1.0, 2.0, 3.0 };
    double ys[] = { 0.0, 1.0, 4.0, 9.0 };
    const int n = 4;

    std::cout << "linearInterpolation(0,0, 2,4, 1) = "
              << linearInterpolation(0.0, 0.0, 2.0, 4.0, 1.0) << std::endl;
    std::cout << "lagrange(xs, ys, 4, 2.5) = " << lagrange(xs, ys, n, 2.5) << std::endl;
    std::cout << "interpolate(xs, ys, 4, 2.5) = " << interpolate(xs, ys, n, 2.5) << std::endl;
    std::cout << "interpolationError(xs, ys, 4, 2.5, 6.25) = "
              << interpolationError(xs, ys, n, 2.5, 6.25) << std::endl;

    std::cout << "\n--- Edge case: looking up a non-existent function ---" << std::endl;
    FARPROC missing = GetProcAddress(library, "thisFunctionDoesNotExist");
    std::cout << "GetProcAddress(\"thisFunctionDoesNotExist\") = "
              << (missing == nullptr ? "nullptr (OK)" : "NOT NULL (unexpected)") << std::endl;

    FreeLibrary(library);
    std::cout << "DLL is unloaded." << std::endl;

    return 0;
}

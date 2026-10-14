#include <iostream>
#include <windows.h> // LoadLibrary, GetProcAddress, FreeLibrary

// Сигнатури, складені власноруч за README.md партнера (PolyUtils), -
// жодного .h файлу партнера тут не підключаємо.
typedef int (*PolyAddFn)(const double*, int, const double*, int, double*, int);
typedef int (*PolyMultiplyFn)(const double*, int, const double*, int, double*, int);
typedef int (*PolyEvaluateFn)(const double*, int, double, double*);
typedef int (*PolyDerivativeFn)(const double*, int, double*, int);
typedef int (*PolyIntegralFn)(const double*, int, double, double*, int);

int main()
{
    std::cout << "=== Explicit linking: PolyUtils (real partner SDK, DLL only) ===" << std::endl;

    HMODULE library = LoadLibraryA("PolyUtils.dll");
    if (library == nullptr)
    {
        std::cerr << "ERROR: Could not load PolyUtils.dll!" << std::endl;
        return 1;
    }
    std::cout << "DLL is successfully loaded!" << std::endl;

    auto polyAdd = reinterpret_cast<PolyAddFn>(GetProcAddress(library, "PolyAdd"));
    auto polyMultiply = reinterpret_cast<PolyMultiplyFn>(GetProcAddress(library, "PolyMultiply"));
    auto polyEvaluate = reinterpret_cast<PolyEvaluateFn>(GetProcAddress(library, "PolyEvaluate"));
    auto polyDerivative = reinterpret_cast<PolyDerivativeFn>(GetProcAddress(library, "PolyDerivative"));
    auto polyIntegral = reinterpret_cast<PolyIntegralFn>(GetProcAddress(library, "PolyIntegral"));

    if (!polyAdd || !polyMultiply || !polyEvaluate || !polyDerivative || !polyIntegral)
    {
        std::cerr << "ERROR: one or more functions were not found in the DLL "
                   << "(check for C++ name mangling / extern \"C\")!" << std::endl;
        FreeLibrary(library);
        return 1;
    }
    std::cout << "All functions were found via GetProcAddress." << std::endl;

    double p[] = { 1.0, 2.0, 3.0 }; // 1 + 2x + 3x^2
    double q[] = { 5.0, 1.0 };      // 5 + x

    double sum[8] = {};
    int sumN = polyAdd(p, 3, q, 2, sum, 8);
    std::cout << "PolyAdd -> code=" << sumN << " result={" << sum[0] << ", " << sum[1] << ", " << sum[2] << "}" << std::endl;

    double mul[8] = {};
    int mulN = polyMultiply(p, 3, q, 2, mul, 8);
    std::cout << "PolyMultiply -> code=" << mulN << " result={";
    for (int i = 0; i < mulN; ++i) std::cout << mul[i] << (i + 1 < mulN ? ", " : "");
    std::cout << "}" << std::endl;

    double value = 0.0;
    int evalStatus = polyEvaluate(p, 3, 2.0, &value);
    std::cout << "PolyEvaluate(p, x=2) -> status=" << evalStatus << " value=" << value << " (expect 0, 17)" << std::endl;

    double deriv[8] = {};
    int derivN = polyDerivative(p, 3, deriv, 8);
    std::cout << "PolyDerivative -> code=" << derivN << " result={" << deriv[0] << ", " << deriv[1] << "}" << std::endl;

    double integ[8] = {};
    int integN = polyIntegral(p, 3, 0.0, integ, 8);
    std::cout << "PolyIntegral -> code=" << integN << " result={";
    for (int i = 0; i < integN; ++i) std::cout << integ[i] << (i + 1 < integN ? ", " : "");
    std::cout << "}" << std::endl;

    std::cout << "\n--- Edge case: nullptr, undersized buffer ---" << std::endl;
    int rNull = polyAdd(nullptr, 3, q, 2, sum, 8);
    std::cout << "PolyAdd(a=nullptr) -> " << rNull << " (expect -1, POLY_ERR_NULL)" << std::endl;

    int rBuf = polyMultiply(p, 3, q, 2, mul, 2);
    std::cout << "PolyMultiply(outCapacity=2, need 4) -> " << rBuf << " (expect -3, POLY_ERR_BUFFER)" << std::endl;

    std::cout << "\n--- Edge case: looking up a non-existent function ---" << std::endl;
    FARPROC missing = GetProcAddress(library, "ThisFunctionDoesNotExist");
    std::cout << "GetProcAddress(\"ThisFunctionDoesNotExist\") = "
              << (missing == nullptr ? "nullptr (OK)" : "NOT NULL (unexpected)") << std::endl;

    FreeLibrary(library);
    std::cout << "DLL is unloaded." << std::endl;

    return 0;
}

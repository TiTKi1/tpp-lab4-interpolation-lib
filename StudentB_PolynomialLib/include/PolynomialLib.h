#pragma once

// Експорт/імпорт символів для динамічної бібліотеки (Dynamic Library).
// Коли бібліотека збирається як STATIC, POLYNOMIALLIB_EXPORTS не визначається
// і POLYNOMIALLIB_API зводиться просто до extern "C" (без dllexport/dllimport).
#if defined(POLYNOMIALLIB_SHARED)
    #ifdef POLYNOMIALLIB_EXPORTS
        #define POLYNOMIALLIB_API extern "C" __declspec(dllexport)
    #else
        #define POLYNOMIALLIB_API extern "C" __declspec(dllimport)
    #endif
#else
    #define POLYNOMIALLIB_API extern "C"
#endif

// =======================================================================
// Контракт бібліотеки PolynomialLib (Варіант 3, Студент Б)
// Версія API: 1.0
//
// Представлення полінома: масив коефіцієнтів double*, де coeffs[i] -
// коефіцієнт при x^i. Розмір масиву size = ступінь полінома + 1.
// Наприклад, поліном 3x^2 + 2x + 1 представляється як
//     coeffs = {1.0, 2.0, 3.0}, size = 3
//
// Керування пам'яттю:
//   - Функції polyAdd/polyMultiply/polyDerivative/polyIntegral виділяють
//     пам'ять під результат САМІ (через new[]) і повертають покажчик.
//   - Користувач бібліотеки ЗОБОВ'ЯЗАНИЙ звільнити цю пам'ять,
//     викликавши polyFreeArray(), а не free()/delete[] напряму.
//   - Якщо вхідні дані некоректні (nullptr або size <= 0), функція
//     повертає nullptr і *outSize встановлюється в 0.
// =======================================================================

// Додавання двох поліномів: result = a + b.
// outSize отримує розмір результату (= max(sizeA, sizeB)).
// Повертає nullptr при некоректних вхідних даних (a == nullptr, b == nullptr,
// sizeA <= 0 або sizeB <= 0).
POLYNOMIALLIB_API double* polyAdd(const double* a, int sizeA,
                                   const double* b, int sizeB,
                                   int* outSize);

// Множення двох поліномів: result = a * b.
// outSize отримує розмір результату (= sizeA + sizeB - 1).
// Повертає nullptr при некоректних вхідних даних.
POLYNOMIALLIB_API double* polyMultiply(const double* a, int sizeA,
                                        const double* b, int sizeB,
                                        int* outSize);

// Обчислення значення полінома в точці x (схема Горнера).
// Якщо coeffs == nullptr або size <= 0, повертає 0.0.
POLYNOMIALLIB_API double polyEvaluate(const double* coeffs, int size, double x);

// Похідна полінома. outSize отримує розмір результату (= size - 1).
// Якщо size == 1 (константа), похідна - поліном {0.0} розміру 1.
// Повертає nullptr при некоректних вхідних даних.
POLYNOMIALLIB_API double* polyDerivative(const double* coeffs, int size, int* outSize);

// Невизначений інтеграл полінома (первісна) з довільною сталою constant.
// outSize отримує розмір результату (= size + 1).
// Повертає nullptr при некоректних вхідних даних.
POLYNOMIALLIB_API double* polyIntegral(const double* coeffs, int size,
                                        double constant, int* outSize);

// Звільнення пам'яті, виділеної polyAdd/polyMultiply/polyDerivative/polyIntegral.
// Безпечно викликати з nullptr (нічого не робить).
POLYNOMIALLIB_API void polyFreeArray(double* array);

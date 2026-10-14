#pragma once

// УВАГА: це MOCK (заглушка) бібліотеки партнера (Студент А, Варіант 3 -
// "Інтерполяція та апроксимація"), потрібна лише для того, щоб
// самостійно перевірити обидва способи зв'язування (Етап 3) ще до того,
// як буде отримано реальний SDK-пакунок від партнера.
// Коли партнер надасть свій README.md (контракт) та SDK, замініть цю
// бібліотеку і каталоги Etap3_ImplicitClient / Etap3_ExplicitClient
// на реальний SDK партнера (зберігши ту саму структуру include/lib/bin).

#if defined(INTERPOLATIONLIB_SHARED)
    #ifdef INTERPOLATIONLIB_EXPORTS
        #define INTERPOLATIONLIB_API extern "C" __declspec(dllexport)
    #else
        #define INTERPOLATIONLIB_API extern "C" __declspec(dllimport)
    #endif
#else
    #define INTERPOLATIONLIB_API extern "C"
#endif

// Лінійна інтерполяція між двома точками (x0, y0) і (x1, y1) в точці x.
// Якщо x0 == x1, повертає y0 (вироджений випадок).
INTERPOLATIONLIB_API double linearInterpolation(double x0, double y0,
                                                  double x1, double y1,
                                                  double x);

// Інтерполяційний поліном Лагранжа за n вузлами (xs[i], ys[i]),
// обчислений у точці x.
// Повертає 0.0, якщо xs == nullptr, ys == nullptr або n <= 0.
INTERPOLATIONLIB_API double lagrange(const double* xs, const double* ys,
                                      int n, double x);

// Кусково-лінійна інтерполяція за масивом вузлів (xs[i], ys[i]),
// відсортованих за зростанням xs, у точці x.
// Якщо x виходить за межі [xs[0], xs[n-1]], виконується екстраполяція
// по найближчому відрізку.
// Повертає 0.0, якщо xs == nullptr, ys == nullptr, n <= 0 або (n == 1
// і x != xs[0], - у цьому випадку теж повертається ys[0]).
INTERPOLATIONLIB_API double interpolate(const double* xs, const double* ys,
                                         int n, double x);

// Похибка інтерполяції - модуль різниці між точним значенням trueValue
// та значенням, отриманим інтерполяцією Лагранжа в точці x.
INTERPOLATIONLIB_API double interpolationError(const double* xs, const double* ys,
                                                int n, double x, double trueValue);

# Лабораторна робота №4 — ТПП

**Розробка статичних та динамічних бібліотек. Явне та неявне зв'язування**

Студент Б, Варіант 3: бібліотека **"Робота з поліномами"**
(`add()`, `multiply()`, `evaluate()`, `derivative()`, `integral()`).

> Звіт пишеться окремо (цей репозиторій містить лише код трьох проєктів,
> вимагає методичка: статична бібліотека, SDK з динамічною бібліотекою
> + контракт, і два тестових проєкти з неявним/явним зв'язуванням).

## Структура репозиторію

```
Lab4/
├── StudentB_PolynomialLib/       Джерела власної бібліотеки (Студент Б).
│                                 Один і той самий код збирається або як
│                                 STATIC (Етап 1), або як SHARED (Етап 2) —
│                                 перемикається прапорцем POLYNOMIALLIB_BUILD_SHARED.
│   ├── include/PolynomialLib.h
│   ├── src/PolynomialLib.cpp
│   └── README_CONTRACT.md        Контракт бібліотеки (передається партнеру)
│
├── Etap1_StudentB_LocalTest/     ЕТАП 1: локальний тест зі статичною бібліотекою.
│   └── main.cpp
│
├── Etap2_SDK_Build/              ЕТАП 2: збирає SHARED-версії обох бібліотек
│   │                             і одразу пакує готові SDK (README+include+lib+bin)
│   │                             у SDK_PolynomialLib/ та SDK_InterpolationMock/.
│   └── CMakeLists.txt
│
├── SDK_PolynomialLib/            Результат збірки Етапу 2 (генерується, не
│                                 редагується руками) — саме цей пакунок
│                                 передається партнеру.
│
├── PartnerLib_InterpolationMock/ MOCK-бібліотека партнера (Студент А, Варіант 3 —
│   │                             "Інтерполяція та апроксимація"). Потрібна лише
│   │                             щоб самостійно перевірити Етап 3 ще до отримання
│   │                             реального SDK від партнера. Коли партнер надішле
│   │                             свій SDK — замініть вміст SDK_InterpolationMock/
│   │                             на отримані файли (структура include/lib/bin
│   │                             та ім'я README.md лишаються тими самими).
│   └── README_CONTRACT.md
│
├── SDK_InterpolationMock/        Результат збірки mock-бібліотеки партнера
│                                 (генерується).
│
├── Etap3_ImplicitClient/         ЕТАП 3, Проєкт 1: НЕЯВНЕ зв'язування.
│                                 Підключає .h і лінкує .lib із SDK_InterpolationMock.
│   └── main.cpp
│
└── Etap3_ExplicitClient/         ЕТАП 3, Проєкт 2: ЯВНЕ зв'язування.
                                  Використовує лише .dll із SDK_InterpolationMock
                                  через LoadLibrary()/GetProcAddress()/FreeLibrary().
    └── main.cpp
```

Кожна з п'яти папок `Etap*` та `Etap2_SDK_Build` — окремий, самостійний CMake-проєкт
(так само, як вимагає методичка — "3 проєкти", кожен можна відкрити окремо
в Visual Studio / CLion / VS Code).

## Порядок збірки

Збирати потрібно саме в такому порядку (Етап 3 залежить від SDK, що
створює Етап 2):

### 1. Візьмемо MinGW (швидко, через командний рядок)

```bash
# Етап 1 — статична бібліотека + локальний тест
cd Etap1_StudentB_LocalTest
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
./build/StudentB_LocalTest.exe

# Етап 2 — динамічні бібліотеки + пакування SDK
cd ../Etap2_SDK_Build
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
# -> з'являться SDK_PolynomialLib/ та SDK_InterpolationMock/ поруч

# Етап 3, Проєкт 1 — неявне зв'язування
cd ../Etap3_ImplicitClient
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
./build/ImplicitClient.exe

# Етап 3, Проєкт 2 — явне зв'язування
cd ../Etap3_ExplicitClient
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
./build/ExplicitClient.exe
```

### 2. Або через Visual Studio

Відкрийте кожну з папок (`Etap1_StudentB_LocalTest`, `Etap2_SDK_Build`,
`Etap3_ImplicitClient`, `Etap3_ExplicitClient`) через
**File → Open → CMake...** у Visual Studio 2022/2026 — CMake-проєкт
визначиться і збереться автоматично (генератор NMake/Ninja з MSVC).
Збирати в тому самому порядку: 1 → 2 → 3.

## Що вже перевірено автоматично

- Коректність `add/multiply/evaluate/derivative/integral` на простому
  прикладі (`p(x)=2x²+3x+1`, `q(x)=x+5`) — Етап 1.
- Граничні випадки (edge cases): `nullptr`, розмір `0`, `polyFreeArray(nullptr)`.
- Неявне зв'язування з SDK партнера (Lagrange, лінійна інтерполяція) — Етап 3.
- Явне зв'язування: `LoadLibrary`/`GetProcAddress`/`FreeLibrary`, включно
  з пошуком неіснуючої функції (`GetProcAddress` повертає `nullptr`).

## Для звіту (що залишається зробити студенту)

Методичка вимагає, щоб у звіті було:
1. Постановка задачі.
2. Опис алгоритмів і складу бібліотеки (готовий код + контракти в `README_CONTRACT.md`).
3. Файлова структура 3 проєктів (скріншоти — зробіть самостійно в Проводнику/VS).
4. Посилання на репозиторій (цей, після `git push`) з доступом викладачу.
5. Оцінка якості документації (README.md) **від реального партнера** —
   тут потрібен справжній обмін SDK з напарником, mock-бібліотека лише
   для самоперевірки.
6. Аналіз граничних випадків на бібліотеці **партнера** (а не mock) —
   після обміну.
7. Скріншоти роботи обох консольних застосунків (Implicit/Explicit) +
   порівняльний аналіз складності.
8. Висновки.

# Лабораторна робота №4 — ТПП

**Розробка статичних та динамічних бібліотек. Явне та неявне зв'язування**

Студент А, Варіант 3: бібліотека **"Інтерполяція та апроксимація"**
(`lagrange()`, `linearInterpolation()`, `interpolate()`, `interpolationError()`).

Партнер (Студент Б, Варіант 3) розробив бібліотеку **"Робота з поліномами"**
(`PolyUtils`: `PolyAdd`, `PolyMultiply`, `PolyEvaluate`, `PolyDerivative`,
`PolyIntegral`) — вона отримана як готовий SDK і лежить у `PolyUtils_SDK/`.

> Звіт пишеться окремо (цей репозиторій містить лише код трьох проєктів,
> як вимагає методичка: статична бібліотека, SDK з динамічною бібліотекою
> + контракт, і два тестових проєкти з неявним/явним зв'язуванням).

## Структура репозиторію

```
Lab4/
├── StudentA_InterpolationLib/    Джерела ВЛАСНОЇ бібліотеки (Студент А).
│                                 Один і той самий код збирається або як
│                                 STATIC (Етап 1), або як SHARED (Етап 2) —
│                                 перемикається прапорцем INTERPOLATIONLIB_BUILD_SHARED.
│   ├── include/InterpolationLib.h
│   ├── src/InterpolationLib.cpp
│   └── README_CONTRACT.md        Контракт бібліотеки (передається партнеру)
│
├── Etap1_StudentA_LocalTest/     ЕТАП 1: локальний тест зі статичною бібліотекою.
│   └── main.cpp
│
├── Etap2_SDK_Build/              ЕТАП 2: збирає SHARED-версію власної бібліотеки
│   │                             і одразу пакує готовий SDK
│   │                             (README+include+lib+bin) у SDK_InterpolationLib/.
│   └── CMakeLists.txt
│
├── SDK_InterpolationLib/         Результат збірки Етапу 2 (генерується, не
│                                 редагується руками) — саме цей пакунок
│                                 передається партнеру (Студенту Б).
│
├── PolyUtils_SDK/                РЕАЛЬНИЙ SDK, отриманий від партнера
│   │                             (Студент Б): README.md (контракт),
│   │                             include/PolyUtils.h, lib/PolyUtils.lib,
│   │                             bin/PolyUtils.dll. Збирався через MSVC
│   │                             (/MD, Release) — при лінкуванні з MinGW
│   │                             проблем не виникло (C-ABI на x64 сумісний).
│   └── README.md                 Контракт партнера
│
├── Etap3_ImplicitClient/         ЕТАП 3, Проєкт 1: НЕЯВНЕ зв'язування.
│                                 Підключає .h і лінкує .lib із PolyUtils_SDK.
│   └── main.cpp
│
└── Etap3_ExplicitClient/         ЕТАП 3, Проєкт 2: ЯВНЕ зв'язування.
                                  Використовує лише .dll із PolyUtils_SDK
                                  через LoadLibrary()/GetProcAddress()/FreeLibrary().
    └── main.cpp
```

Кожна з папок `Etap*` та `Etap2_SDK_Build` — окремий, самостійний CMake-проєкт
(так само, як вимагає методичка — "3 проєкти", кожен можна відкрити окремо
в Visual Studio / CLion / VS Code).

## Порядок збірки

Збирати потрібно саме в такому порядку (Етап 3 залежить від SDK, що
створює Етап 2; SDK партнера вже лежить готовим у `PolyUtils_SDK/`):

### 1. Через MinGW (швидко, через командний рядок)

```bash
# Етап 1 — статична бібліотека + локальний тест
cd Etap1_StudentA_LocalTest
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
./build/StudentA_LocalTest.exe

# Етап 2 — динамічна бібліотека + пакування SDK
cd ../Etap2_SDK_Build
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
# -> з'явиться SDK_InterpolationLib/ поруч

# Етап 3, Проєкт 1 — неявне зв'язування з PolyUtils_SDK (партнер)
cd ../Etap3_ImplicitClient
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
./build/ImplicitClient.exe

# Етап 3, Проєкт 2 — явне зв'язування з PolyUtils_SDK (партнер)
cd ../Etap3_ExplicitClient
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
./build/ExplicitClient.exe
```

### 2. Або через Visual Studio

Відкрийте кожну з папок (`Etap1_StudentA_LocalTest`, `Etap2_SDK_Build`,
`Etap3_ImplicitClient`, `Etap3_ExplicitClient`) через
**File → Open → CMake...** у Visual Studio — CMake-проєкт визначиться і
збереться автоматично. Збирати в тому самому порядку: 1 → 2 → 3.

## Що вже перевірено автоматично

- Коректність `lagrange/linearInterpolation/interpolate/interpolationError`
  на вузлах `(0,0),(1,1),(2,4),(3,9)` (наближення `y=x²`) — Етап 1.
- Граничні випадки власної бібліотеки: `nullptr`, `n=0`, виродженість `x0==x1`.
- Неявне зв'язування з реальним SDK партнера (`PolyAdd/Multiply/Evaluate/
  Derivative/Integral`) — усі значення збігаються з контрактом партнера.
- Граничні випадки партнера (за таблицею в `PolyUtils_SDK/README.md`,
  розділ 7): `POLY_ERR_NULL`, `POLY_ERR_SIZE`, `POLY_ERR_BUFFER`, похідна
  константи, незгорнутий старший нульовий коефіцієнт, `NaN`.
- Явне зв'язування: `LoadLibrary`/`GetProcAddress`/`FreeLibrary`, включно
  з пошуком неіснуючої функції (`GetProcAddress` повертає `nullptr`).

## Для звіту (що залишається зробити студенту)

Методичка вимагає, щоб у звіті було:
1. Постановка задачі.
2. Опис алгоритмів і складу бібліотеки (готовий код + контракт у
   `StudentA_InterpolationLib/README_CONTRACT.md`).
3. Файлова структура 3 проєктів (скріншоти — зробіть самостійно в
   Проводнику/VS).
4. Посилання на репозиторій (після `git push`) з доступом викладачу.
5. Оцінка якості документації (README.md) **партнера** — вже доступна
   в `PolyUtils_SDK/README.md`, залишилось написати власну думку про
   її зрозумілість, повноту й однозначність.
6. Аналіз граничних випадків на бібліотеці партнера — вже протестовано
   в `Etap3_ImplicitClient/main.cpp` та `Etap3_ExplicitClient/main.cpp`,
   залишилось оформити результати в звіті.
7. Скріншоти роботи обох консольних застосунків (Implicit/Explicit) +
   порівняльний аналіз складності розробки.
8. Висновки.

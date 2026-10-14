#include "PolynomialLib.h"

double* polyAdd(const double* a, int sizeA, const double* b, int sizeB, int* outSize)
{
    if (!outSize)
    {
        return nullptr;
    }
    *outSize = 0;

    if (a == nullptr || b == nullptr || sizeA <= 0 || sizeB <= 0)
    {
        return nullptr;
    }

    const int resultSize = (sizeA > sizeB) ? sizeA : sizeB;
    double* result = new double[resultSize];

    for (int i = 0; i < resultSize; ++i)
    {
        const double termA = (i < sizeA) ? a[i] : 0.0;
        const double termB = (i < sizeB) ? b[i] : 0.0;
        result[i] = termA + termB;
    }

    *outSize = resultSize;
    return result;
}

double* polyMultiply(const double* a, int sizeA, const double* b, int sizeB, int* outSize)
{
    if (!outSize)
    {
        return nullptr;
    }
    *outSize = 0;

    if (a == nullptr || b == nullptr || sizeA <= 0 || sizeB <= 0)
    {
        return nullptr;
    }

    const int resultSize = sizeA + sizeB - 1;
    double* result = new double[resultSize];
    for (int i = 0; i < resultSize; ++i)
    {
        result[i] = 0.0;
    }

    for (int i = 0; i < sizeA; ++i)
    {
        for (int j = 0; j < sizeB; ++j)
        {
            result[i + j] += a[i] * b[j];
        }
    }

    *outSize = resultSize;
    return result;
}

double polyEvaluate(const double* coeffs, int size, double x)
{
    if (coeffs == nullptr || size <= 0)
    {
        return 0.0;
    }

    // Схема Горнера: обчислення від старшого коефіцієнта до молодшого.
    double result = coeffs[size - 1];
    for (int i = size - 2; i >= 0; --i)
    {
        result = result * x + coeffs[i];
    }
    return result;
}

double* polyDerivative(const double* coeffs, int size, int* outSize)
{
    if (!outSize)
    {
        return nullptr;
    }
    *outSize = 0;

    if (coeffs == nullptr || size <= 0)
    {
        return nullptr;
    }

    if (size == 1)
    {
        // Похідна константи дорівнює 0.
        double* result = new double[1];
        result[0] = 0.0;
        *outSize = 1;
        return result;
    }

    const int resultSize = size - 1;
    double* result = new double[resultSize];
    for (int i = 1; i < size; ++i)
    {
        result[i - 1] = coeffs[i] * static_cast<double>(i);
    }

    *outSize = resultSize;
    return result;
}

double* polyIntegral(const double* coeffs, int size, double constant, int* outSize)
{
    if (!outSize)
    {
        return nullptr;
    }
    *outSize = 0;

    if (coeffs == nullptr || size <= 0)
    {
        return nullptr;
    }

    const int resultSize = size + 1;
    double* result = new double[resultSize];
    result[0] = constant;
    for (int i = 0; i < size; ++i)
    {
        result[i + 1] = coeffs[i] / static_cast<double>(i + 1);
    }

    *outSize = resultSize;
    return result;
}

void polyFreeArray(double* array)
{
    delete[] array;
}

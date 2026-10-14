#include <iostream>
#include "PolynomialLib.h"

static void printPoly(const double* coeffs, int size)
{
    if (coeffs == nullptr || size <= 0)
    {
        std::cout << "(null)";
        return;
    }
    for (int i = size - 1; i >= 0; --i)
    {
        std::cout << coeffs[i];
        if (i > 0) std::cout << "*x^" << i << " + ";
    }
}

int main()
{
    // p(x) = 2x^2 + 3x + 1
    double p[] = { 1.0, 3.0, 2.0 };
    int pSize = 3;

    // q(x) = x + 5
    double q[] = { 5.0, 1.0 };
    int qSize = 2;

    std::cout << "p(x) = "; printPoly(p, pSize); std::cout << std::endl;
    std::cout << "q(x) = "; printPoly(q, qSize); std::cout << std::endl;

    int sumSize = 0;
    double* sum = polyAdd(p, pSize, q, qSize, &sumSize);
    std::cout << "p + q = "; printPoly(sum, sumSize); std::cout << std::endl;
    polyFreeArray(sum);

    int mulSize = 0;
    double* mul = polyMultiply(p, pSize, q, qSize, &mulSize);
    std::cout << "p * q = "; printPoly(mul, mulSize); std::cout << std::endl;
    polyFreeArray(mul);

    std::cout << "p(2) = " << polyEvaluate(p, pSize, 2.0) << " (expect 15)" << std::endl;

    int derivSize = 0;
    double* deriv = polyDerivative(p, pSize, &derivSize);
    std::cout << "p'(x) = "; printPoly(deriv, derivSize); std::cout << std::endl;
    polyFreeArray(deriv);

    int integSize = 0;
    double* integ = polyIntegral(p, pSize, 0.0, &integSize);
    std::cout << "Integral p(x) dx = "; printPoly(integ, integSize); std::cout << std::endl;
    polyFreeArray(integ);

    std::cout << "\n--- Edge cases ---" << std::endl;

    int badSize = -1;
    double* badAdd = polyAdd(nullptr, 3, q, qSize, &badSize);
    std::cout << "polyAdd(nullptr, ...) -> " << (badAdd == nullptr ? "nullptr (OK)" : "NOT NULL (FAIL)")
              << ", outSize=" << badSize << std::endl;

    double* badMul = polyMultiply(p, 0, q, qSize, &badSize);
    std::cout << "polyMultiply(size=0, ...) -> " << (badMul == nullptr ? "nullptr (OK)" : "NOT NULL (FAIL)")
              << ", outSize=" << badSize << std::endl;

    std::cout << "polyEvaluate(nullptr, 0, 5.0) -> " << polyEvaluate(nullptr, 0, 5.0) << " (expect 0)" << std::endl;

    polyFreeArray(nullptr);
    std::cout << "polyFreeArray(nullptr) -> OK, no crash" << std::endl;

    return 0;
}

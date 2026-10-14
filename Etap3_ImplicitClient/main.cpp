#include <iostream>
#include <cmath>
#include <limits>
#include "PolyUtils.h" // Неявне зв'язування: заголовок + .lib реального SDK партнера

static const char* codeName(int code)
{
    switch (code)
    {
        case POLY_ERR_NULL:   return "POLY_ERR_NULL";
        case POLY_ERR_SIZE:   return "POLY_ERR_SIZE";
        case POLY_ERR_BUFFER: return "POLY_ERR_BUFFER";
        default:              return "(success / size)";
    }
}

int main()
{
    std::cout << "=== Implicit linking: PolyUtils (real partner SDK) ===" << std::endl;

    // p(x) = 1 + 2x + 3x^2
    double p[] = { 1.0, 2.0, 3.0 };
    const int pSize = 3;

    // q(x) = 5 + x
    double q[] = { 5.0, 1.0 };
    const int qSize = 2;

    double sum[8] = {};
    int sumN = PolyAdd(p, pSize, q, qSize, sum, 8);
    std::cout << "PolyAdd -> code=" << sumN << " result={" << sum[0] << ", " << sum[1] << ", " << sum[2] << "} (expect {6,3,3})" << std::endl;

    double mul[8] = {};
    int mulN = PolyMultiply(p, pSize, q, qSize, mul, 8);
    std::cout << "PolyMultiply -> code=" << mulN << " result={";
    for (int i = 0; i < mulN; ++i) std::cout << mul[i] << (i + 1 < mulN ? ", " : "");
    std::cout << "} (expect {5,11,17,3})" << std::endl;

    double value = 0.0;
    int evalStatus = PolyEvaluate(p, pSize, 2.0, &value);
    std::cout << "PolyEvaluate(p, x=2) -> status=" << evalStatus << " value=" << value << " (expect status=0, value=17)" << std::endl;

    double deriv[8] = {};
    int derivN = PolyDerivative(p, pSize, deriv, 8);
    std::cout << "PolyDerivative -> code=" << derivN << " result={" << deriv[0] << ", " << deriv[1] << "} (expect {2,6})" << std::endl;

    double integ[8] = {};
    int integN = PolyIntegral(p, pSize, 0.0, integ, 8);
    std::cout << "PolyIntegral -> code=" << integN << " result={";
    for (int i = 0; i < integN; ++i) std::cout << integ[i] << (i + 1 < integN ? ", " : "");
    std::cout << "} (expect {0, 1, 1, 1})" << std::endl;

    std::cout << "\n--- Edge cases (per partner's README section 7) ---" << std::endl;

    double buf[8] = {};

    int r1 = PolyAdd(nullptr, pSize, q, qSize, buf, 8);
    std::cout << "PolyAdd(a=nullptr) -> " << r1 << " (" << codeName(r1) << "), expect POLY_ERR_NULL" << std::endl;

    int r2 = PolyEvaluate(p, 0, 2.0, &value);
    std::cout << "PolyEvaluate(n=0) -> " << r2 << " (" << codeName(r2) << "), expect POLY_ERR_SIZE" << std::endl;

    int r3 = PolyMultiply(p, pSize, q, qSize, buf, 2);
    std::cout << "PolyMultiply(outCapacity=2, need 4) -> " << r3 << " (" << codeName(r3) << "), expect POLY_ERR_BUFFER" << std::endl;

    double constZero[] = { 7.0 };
    double derivConst[4] = {};
    int r4 = PolyDerivative(constZero, 1, derivConst, 4);
    std::cout << "PolyDerivative(constant) -> code=" << r4 << " result={" << derivConst[0] << "} (expect code=1, {0})" << std::endl;

    // a(x) = 1 + 2x + 0*x^2 (старший коефіцієнт - 0, але все одно 3 елементи)
    double topZero[] = { 1.0, 2.0, 0.0 };
    double one[] = { 1.0 };
    double sumTZ[4] = {};
    int r5 = PolyAdd(topZero, 3, one, 1, sumTZ, 4);
    std::cout << "PolyAdd, top coeff=0 not trimmed -> code=" << r5
              << " result={" << sumTZ[0] << ", " << sumTZ[1] << ", " << sumTZ[2]
              << "} (expect code=3, {2,2,0})" << std::endl;

    double nanValue = 0.0;
    double nanPoly[] = { std::numeric_limits<double>::quiet_NaN(), 1.0 };
    int r6 = PolyEvaluate(nanPoly, 2, 1.0, &nanValue);
    std::cout << "PolyEvaluate with NaN coefficient -> status=" << r6
              << " value=" << nanValue << " (NaN expected, no dedicated error code per contract)" << std::endl;

    return 0;
}

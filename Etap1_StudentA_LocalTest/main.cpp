#include <iostream>
#include "InterpolationLib.h"

int main()
{
    // Вузли: (0,0), (1,1), (2,4), (3,9) - наближено y = x^2
    double xs[] = { 0.0, 1.0, 2.0, 3.0 };
    double ys[] = { 0.0, 1.0, 4.0, 9.0 };
    const int n = 4;

    std::cout << "linearInterpolation(0,0, 2,4, 1) = "
              << linearInterpolation(0.0, 0.0, 2.0, 4.0, 1.0)
              << " (expect 2)" << std::endl;

    std::cout << "lagrange(xs, ys, 4, 2.5) = "
              << lagrange(xs, ys, n, 2.5)
              << " (expect 6.25, true x^2 at 2.5)" << std::endl;

    std::cout << "interpolate(xs, ys, 4, 2.5) = "
              << interpolate(xs, ys, n, 2.5)
              << " (piecewise-linear estimate)" << std::endl;

    std::cout << "interpolationError(xs, ys, 4, 2.5, 6.25) = "
              << interpolationError(xs, ys, n, 2.5, 6.25) << std::endl;

    std::cout << "\n--- Edge cases ---" << std::endl;
    std::cout << "lagrange(nullptr, nullptr, 0, 1.0) = " << lagrange(nullptr, nullptr, 0, 1.0)
              << " (expect 0)" << std::endl;
    std::cout << "linearInterpolation(5,1, 5,9, 7) = " << linearInterpolation(5.0, 1.0, 5.0, 9.0, 7.0)
              << " (degenerate x0==x1, expect y0=1)" << std::endl;
    std::cout << "interpolate(xs, ys, 1, 100.0) = " << interpolate(xs, ys, 1, 100.0)
              << " (single node, expect ys[0]=0)" << std::endl;

    return 0;
}

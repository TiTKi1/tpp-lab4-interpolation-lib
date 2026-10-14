#include "InterpolationLib.h"

double linearInterpolation(double x0, double y0, double x1, double y1, double x)
{
    if (x1 == x0)
    {
        return y0;
    }
    const double t = (x - x0) / (x1 - x0);
    return y0 + t * (y1 - y0);
}

double lagrange(const double* xs, const double* ys, int n, double x)
{
    if (xs == nullptr || ys == nullptr || n <= 0)
    {
        return 0.0;
    }

    double result = 0.0;
    for (int i = 0; i < n; ++i)
    {
        double term = ys[i];
        for (int j = 0; j < n; ++j)
        {
            if (j == i)
            {
                continue;
            }
            if (xs[i] == xs[j])
            {
                // Вироджений випадок: однакові вузли по x.
                continue;
            }
            term *= (x - xs[j]) / (xs[i] - xs[j]);
        }
        result += term;
    }
    return result;
}

double interpolate(const double* xs, const double* ys, int n, double x)
{
    if (xs == nullptr || ys == nullptr || n <= 0)
    {
        return 0.0;
    }
    if (n == 1)
    {
        return ys[0];
    }

    // Знаходимо відрізок [xs[i], xs[i+1]], якому належить x
    // (за умови, що вузли відсортовані за зростанням).
    int i = 0;
    while (i < n - 2 && x > xs[i + 1])
    {
        ++i;
    }

    return linearInterpolation(xs[i], ys[i], xs[i + 1], ys[i + 1], x);
}

double interpolationError(const double* xs, const double* ys, int n, double x, double trueValue)
{
    const double approx = lagrange(xs, ys, n, x);
    const double diff = approx - trueValue;
    return diff < 0.0 ? -diff : diff;
}

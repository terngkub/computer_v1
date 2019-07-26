#include "math.hpp"

double power(double base, int power)
{
    if (power == 0)
        return 1;
    double ret = base;
    while (--power)
        ret *= base;
    return ret;
}

double sqrt(double number)
{
    double x = 0;
    while (x * x <= number)
        x += 0.1;

    double prev_x = x + 1;
    while (prev_x - x >= 0.000001)
    {
        prev_x = x;
        x = (x + number / x) / 2;
    }
    return x;
}

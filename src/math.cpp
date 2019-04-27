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
    double x;
    double new_x;

    x = 0;
    while (x * x <= number)
        x += 0.1;
    for (auto i = 0; i < 10; ++i)
    {
        new_x = (x + number / x) / 2;
        x = new_x;
    }
    return new_x;
}

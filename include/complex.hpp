#pragma once
#include <ostream>

struct Complex
{
    double rational;
    double imaginary;
    
    Complex();
    Complex(double, double);
};

std::ostream & operator<<(std::ostream & o, Complex const & c);

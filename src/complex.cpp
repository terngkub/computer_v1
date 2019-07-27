#include "complex.hpp"

Complex::Complex() :
    rational(0),
    imaginary(0)
{}

Complex::Complex(double rational, double imaginary) :
    rational(rational),
    imaginary(imaginary)
{}

std::ostream & operator<<(std::ostream & o, Complex const & c)
{
    if (c.imaginary == 0)
    {
        o << c.rational;
        return o;
    }

    if (c.rational != 0)
    {
        o << c.rational;
        if (c.imaginary < 0)
            o << " - " << -c.imaginary << "i";
        else
            o << " + " << c.imaginary << "i";
    }
    else
        o << c.imaginary << "i";
    return o;
}
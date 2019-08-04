#pragma once
#include <string>

struct Output
{
    std::string reduced_form;
    int degree;
    bool has_limit;
    std::string solution;
    std::string error;

    Output();
    Output(std::string error);
};

std::ostream & operator<<(std::ostream & o, Output const & output);
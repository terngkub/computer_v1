# Computor V1
a calculator and 2-degree polynomial equation solver in C++

## Usage
1) build the program
    ```
    make
    ```
2) run program
    ```
    ./computor "[input]"
    # example
    ./computor "x^2 + 3x + 2 = 0"
    ```
3) test program
    ```
    ./computor_test
    ```

## Features
The program can be
1) a calculator by inputting an expression without an equal size
    ```
    ./computor "(3+4)*2^5
    ```
2) a upto-2-degree polynomial equation solver by inputting an polynomial expression
    Remark: if the input don't have an eqaul size, the program will interpret it as "= 0" in this case.
    ```
    ./computor "x^2 + 3x + 2 = 0"
    ```

The program can handle:
* real numbers (as C++ double type)
    ```
    42, 0, -1, 1.23
    ```
* one variable (can contain either uppercase or lowercase alphabet)
    ```
    x, X, abc, Abc, aBc
    ```
* operator +-*/^()=
* natural form
    ```
    2x^2 + 3x + 1 = 0
    ```
* polynomial equation solution(s) with complex number
    ```
    $ ./computor "x^2 + 2x + 2 = 0"
    parsed form  : ((((x^2) + (2 * x)) + 2) = 0)
    reduced form : x^2 + 2x + 2 = 0
    degree       : 2
    solution:    : x = -1 + i, -1 - i
    ```
* more-than-2-degree polynomial equation which can be reduced to 2-or-less degree by division of x
    ```
    ./computor "x^3 + 3x^2 + 2x = 0"
    parsed form  : ((((x^3) + (3 * (x^2))) + (2 * x)) = 0)
    reduced form : (x)(x^2 + 3x + 2) = 0
    degree       : 3
    solution:    : x = -1, -2, 0
    ```
* polynomial equation with division
    ```
    ./computor "1 - 4/x^2 = 0"
    parsed form  : ((1 - (4 / (x^2))) = 0)
    reduced form : x^2 - 4 = 0
    degree       : 2
    limitation   : x != 0
    solution:    : x = 2, -2
    ```

## Limitations
* denominator cannot be an expression that contain both variable(s) and number e.g. `10/x^2` is okay but `10/(x + 1)` is invalid .
    ```
    10/x^2      #ok
    10/(x+1)    #invalid
    ```
* degree can only be positive number
    ```
    x^(1+2) #ok
    x^x     #invalid
    ```

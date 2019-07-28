# Computor V1
a calculator and 2-degree polynomial equation solver

## Features
The calculator can handle:
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
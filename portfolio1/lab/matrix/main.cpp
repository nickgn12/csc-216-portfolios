#include <iostream>
#include "matrix.hpp"

int main( )
{
    Matrix<3,2> a = { 1, 2, 3, 4, 5, 6 };
    Matrix<2,3> b = { 7, 8, 9, 10, 11, 12, 13 };
    Matrix<2,3> c = { 14, 15, 16, 17, 18, 19 };

    std::cout << "Matrices: " << std::endl;
    std::cout << "a:   " << a << std::endl << "b:   " << b << std::endl << "c:   " << c << std::endl << std::endl;

    auto product = a * b;
    auto sum = b + c;
    
    std::cout << "a*b: " << product << std::endl;
    std::cout << "b+c: " << sum << std::endl;
    
    return 0;
}

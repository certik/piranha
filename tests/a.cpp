/*
   Compile and run for example with:

   g++ -Wall -Wextra -fPIC -g -std=c++11 a.cpp && ./a.out

*/

#include <iostream>
#include "a.h"

int main()
{
    myint i;
    myint x(1);
    i = 5;
    std::cout << "ok" << std::endl;
    std::cout << i << std::endl;
    bool b = i == myint(0);
    std::cout << b << std::endl;
    b = i != myint(1);
    std::cout << b << std::endl;
    myint r;
    r = i+x;
    i += x;
    r = i-x;
    i -= x;
    x = -x;
    return 0;
}

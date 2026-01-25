extern "C" {
#include "some_c_utility.h"
}

#include "some_cpp_utility.hpp"

#include <iostream>
#include <cstdlib>


int main()
{
    std::cout << "Hello, world!" << std::endl;
    std::cout << "2 + 2 = " << add(2, 2) << std::endl;
    std::cout << "2 × 3 = " << mult(2, 3) << std::endl;

    return EXIT_SUCCESS;
}

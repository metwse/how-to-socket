extern "C" {
#include "some_c_utility.h"
}

#include "some_cpp_utility.hpp"

#include <iostream>
#include <cstdlib>

using std::cout, std::endl;


int main() {
    cout << "Hello, world!" << endl;
    cout << "2 + 2 = " << add(2, 2) << endl;
    cout << "2 × 3 = " << mult(2, 3) << endl;

    return EXIT_SUCCESS;
}

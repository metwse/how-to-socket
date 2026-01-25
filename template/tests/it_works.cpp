#include "../src/some_cpp_utility.hpp"

#include <cassert>
#include <cstdlib>


int main()
{
    assert(mult(2, 2) == 4);
    assert(mult(1, 2) == 2);

    return EXIT_SUCCESS;
}

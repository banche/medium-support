#include "lib.hpp"

int main(int argc, const char** argv)
{
    const auto res = ctest_pytest::add(20, 22);

    return res != 42;
}

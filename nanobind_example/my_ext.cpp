#include "nanobind/nanobind.h"

int add(int x, int y)
{
    return x + y;
}

NB_MODULE(my_ext, m)
{
    m.doc() = "This is a simple python extension exposing our C++ types & functions";

    m.def("add", &add, "Adds two integers");
}
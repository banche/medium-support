#include "nanobind/nanobind.h"

namespace {
int add(int x, int y)
{
    return x + y;
}
}

NB_MODULE(_pylib, m)
{
    m.def("add", &add);
}

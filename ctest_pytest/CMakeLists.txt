cmake_minimum_required(VERSION 3.27)
project(ctest_pytest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#enable ctest
include(CTest)
enable_testing()

add_library(lib lib.cpp)

add_executable(test_lib test_lib.cpp)
target_link_libraries(test_lib lib)
add_test(NAME test_lib COMMAND test_lib)

find_package(Python 3.12 REQUIRED COMPONENTS Interpreter Development)
# find nanobind
execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE nanobind_ROOT)
find_package(nanobind CONFIG REQUIRED)


add_subdirectory(pylib)

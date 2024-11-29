add_subdirectory(cpp)


add_test(NAME pylib COMMAND ${PYTHON_EXECUTABLE} pytest ${CMAKE_CURRENT_SOURCE_DIR}/pylib)

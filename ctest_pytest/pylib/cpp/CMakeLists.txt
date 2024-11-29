nanobind_add_module(_pylib bindings.cpp)
nanobind_add_stub(
    _pylib_stub 
    MODULE _pylib 
    OUTPUT _pylib.pyi 
    PYTHON_PATH $<TARGET_FILE_DIR:_pylib>
    DEPENDS _pylib
)

add_custom_command(
    TARGET _pylib
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE:_pylib> ${CMAKE_CURRENT_SOURCE_DIR}/../src/pylib/$<TARGET_FILE_NAME:_pylib>
    COMMENT "Creating symlink in the source for $<TARGET_FILE_NAME:_pylib>"
    VERBATIM
)

add_custom_command(
    TARGET _pylib_stub
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/_pylib.pyi ${CMAKE_CURRENT_SOURCE_DIR}/../src/pylib/_pylib.pyi
    COMMENT "Creating stub symlink in the sources"
    VERBATIM
)


# Not adding header directory to include path, because that directory defines interfaces for which
# the port needs to provide a header, and possibly a source file. If we add the header directory
# to the include path, we include both the interface definition header and the implementation header,
# and that can get ugly because the same macros defined in both files.
target_include_directories(eas_app INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/osal
)

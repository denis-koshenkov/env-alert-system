add_executable(test_exec1)

target_sources(test_exec1 PRIVATE
    main.cpp
    value_holder.cpp
)

target_link_libraries(test_exec1 PRIVATE test_common)

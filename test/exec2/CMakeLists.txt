add_executable(test_exec2)

target_sources(test_exec2 PRIVATE
    main.cpp
    temperature_value.cpp
)

target_link_libraries(test_exec2 PRIVATE test_common)

add_executable(test_database
    test_database.cpp
)
target_link_libraries(test_database
    sqlite
    gtest
    gtest_main
)
add_test(test_database
    test_database
)

add_executable(test_statement
    test_statement.cpp
)
target_link_libraries(test_statement
    sqlite
    gtest
    gtest_main
)
add_test(test_statement
    test_statement
)

add_executable(test_result
    test_result.cpp
)
target_link_libraries(test_result
    sqlite
    gtest
    gtest_main
)
add_test(test_result
    test_result
)

add_executable(test_row
    test_row.cpp
)
target_link_libraries(test_row
    sqlite
    gtest
    gtest_main
)
add_test(test_row
    test_row
)

add_executable(test_field
    test_field.cpp
)
target_link_libraries(test_field
    sqlite
    gtest
    gtest_main
)
add_test(test_field
    test_field
)

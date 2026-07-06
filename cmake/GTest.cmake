set(CMAKE_SKIP_TEST_ALL_DEPENDENCY FALSE)
enable_testing()

find_package(GTest REQUIRED)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    test/*.cpp
)
list(SORT TEST_SOURCES)

add_executable(${NAME}-tests
    ${TEST_SOURCES}
)
target_link_libraries(${NAME}-tests PRIVATE
    ${NAMESPACE}::${NAME}
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${NAME}-tests)

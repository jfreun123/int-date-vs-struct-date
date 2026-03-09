# ---- Tests ----

add_executable(date_test source/date_test.cpp)
target_link_libraries(date_test PRIVATE int-vs-struct_lib GTest::gtest_main)
target_compile_features(date_test PRIVATE cxx_std_23)

set_target_properties(date_test PROPERTIES
    CXX_CLANG_TIDY "clang-tidy"
    CXX_CPPCHECK "cppcheck;--inline-suppr"
)

include(GoogleTest)
gtest_discover_tests(date_test)

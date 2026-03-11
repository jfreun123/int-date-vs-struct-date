# ---- Tests ----

foreach(target date_common_test date_v1_test date_v2_test date_v3_test)
  add_executable(${target} source/${target}.cpp)
  target_link_libraries(${target} PRIVATE int-vs-struct_lib GTest::gtest_main)
  target_compile_features(${target} PRIVATE cxx_std_23)
  set_target_properties(${target} PROPERTIES
      CXX_CLANG_TIDY "clang-tidy"
      CXX_CPPCHECK "cppcheck;--inline-suppr"
  )
  include(GoogleTest)
  gtest_discover_tests(${target})
endforeach()

# ---- Tests ----

include(CTest)
if(BUILD_TESTING)
  include(cmake/gtest.cmake)
  add_subdirectory(test)
endif()

# ---- Benchmark ----

include(cmake/gbenchmark.cmake)
add_subdirectory(benchmark)

# ---- Linting (clang-format) ----

include(cmake/lint-targets.cmake)

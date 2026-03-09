# ---- Benchmark ----

add_executable(int-vs-struct_benchmark source/benchmark.cpp)
target_link_libraries(int-vs-struct_benchmark PRIVATE int-vs-struct_lib benchmark::benchmark)
target_compile_features(int-vs-struct_benchmark PRIVATE cxx_std_23)

set_target_properties(int-vs-struct_benchmark PROPERTIES
    CXX_CLANG_TIDY "clang-tidy"
)

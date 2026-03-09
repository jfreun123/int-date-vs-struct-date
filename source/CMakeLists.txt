# ---- Library ----

add_library(int-vs-struct_lib INTERFACE)

target_include_directories(
    int-vs-struct_lib
    INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(int-vs-struct_lib INTERFACE cxx_std_23)

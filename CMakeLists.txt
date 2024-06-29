cmake_minimum_required(VERSION 3.24.0)

project(RayTracingInOneWeekend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,AppleClang,Clang,GNU>")

set(SOURCES
        src/sphere.cc
        src/hittable_list.cc
        src/camera.cc
        src/color.cc
        src/config.cc
        src/aabb.cc
        src/bvhnode.cc
        src/main.cc)

add_executable(ray_tracing_in_one_weekend ${SOURCES})

target_include_directories(ray_tracing_in_one_weekend
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(ray_tracing_in_one_weekend PRIVATE
  "$<${gcc_like_cxx}:-Wall;-Wextra;-Wconversion;-Wdouble-promotion;-Wno-unused-parameter;-Wno-unused-function;-Wno-sign-conversion>"
)



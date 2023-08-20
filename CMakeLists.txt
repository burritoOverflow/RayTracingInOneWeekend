cmake_minimum_required(VERSION 3.25.0)

project(RayTracingInOneWeekend LANGUAGES CXX)

include_directories(src)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(SOURCES
        src/main.cc
        )

add_executable(ray_tracing_in_one_weekend ${SOURCES})

target_include_directories(ray_tracing_in_one_weekend
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )
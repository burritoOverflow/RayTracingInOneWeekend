cmake_minimum_required(VERSION 3.25.0)

project(RayTracingInOneWeekend LANGUAGES CXX)

include_directories(src)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(SOURCES
  src/main.cc
)

add_executable(raytracinginoneweekend ${SOURCES})

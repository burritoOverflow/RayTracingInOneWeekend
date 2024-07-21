cmake_minimum_required(VERSION 3.24.0)

include(FetchContent)

set(TARGET_NAME RayTracingInOneWeekend)

project(${TARGET_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(GCC_LIKE_CXX "$<COMPILE_LANG_AND_ID:CXX,AppleClang,Clang,GNU>")

FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master
        SOURCE_DIR ${CMAKE_BINARY_DIR}/external
)

FetchContent_MakeAvailable(stb)

add_library(stb INTERFACE)

set(SOURCES
        src/sphere.cc
        src/hittable_list.cc
        src/camera.cc
        src/color.cc
        src/config.cc
        src/texture.cc
        src/hittable.cc
        src/perlin.cc
        src/aabb.cc
        src/args.cc
        src/quad.cc
        src/image_utils.cc
        src/bvhnode.cc
        src/main.cc)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
        PRIVATE
        stb ${stb_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(${PROJECT_NAME} PRIVATE
        "$<${GCC_LIKE_CXX}:-Wall;-Wextra;-pedantic;-Wconversion;-Wdouble-promotion;-Wno-unused-parameter;-Wno-unused-function;-Wno-sign-conversion;-Wmissing-field-initializers>"
)

set(IMAGE_DIR "texture_images")
target_compile_definitions(${PROJECT_NAME} PRIVATE DEFAULT_IMAGE_DIR="${IMAGE_DIR}")

# for convenience, copy the $IMAGE_DIR from srctree to buildtree
# the expectation is that $IMAGE_DIR is in the same wd as where the executable is run
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$<TARGET_PROPERTY:${PROJECT_NAME},SOURCE_DIR>/${IMAGE_DIR}"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${IMAGE_DIR}"
)


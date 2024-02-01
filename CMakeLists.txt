cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

set(CMAKE_GENERATOR_TOOLSET "ClangCL")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PSET4_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

project(pset4 VERSION 0.1.0)

# source file
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Source" FILES ${HEADER_FILES} ${SOURCE_FILES})
include_directories(${PSET4_ROOT_DIR}/source)

# winpthreads
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/winpthreads)
target_include_directories(winpthreads PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/winpthreads)
link_libraries(winpthreads)

# thirdparty
include_directories(${PSET4_ROOT_DIR}/thirdparty/spdlog)
include_directories(${PSET4_ROOT_DIR}/thirdparty/cxxopts)

# defines
add_definitions(-DPSET_DEBUG)

# executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${SOURCE_FILES_CC} ${SOURCE_FILES_C})
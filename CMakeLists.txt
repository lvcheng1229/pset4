cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

set(CMAKE_GENERATOR_TOOLSET "ClangCL")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PSET4_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

project(pset4 VERSION 0.1.0)

include (TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
 message(STATUS "BIG_ENDIAN")
 add_definitions(-DLITTLEENDIAN_CPU)
else()
 message(STATUS "LITTLE_ENDIAN")
 add_definitions(-DLITTLEENDIAN_CPU)
endif()

# source file
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Source" FILES ${HEADER_FILES} ${SOURCE_FILES})
include_directories(${PSET4_ROOT_DIR}/source)


# rtaudio
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/rtaudio)
include_directories(${PSET4_ROOT_DIR}/thirdparty/rtaudio)
link_directories(${PSET4_ROOT_DIR}/thirdparty/rtaudio/lib_pset4)
link_libraries(rtaudiod)

# vulkan
include_directories(${PSET4_ROOT_DIR}/thirdparty/vulkan/include)
link_directories(${PSET4_ROOT_DIR}/thirdparty/vulkan/Lib)
link_libraries(vulkan-1)

# glfw
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/glfw)
link_libraries(glfw)

# winpthreads
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/winpthreads)
target_include_directories(winpthreads PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/winpthreads)
link_libraries(winpthreads)

# amd vulkan memory allocator
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/VulkanMemoryAllocator)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/VulkanMemoryAllocator/include)
link_libraries(VulkanMemoryAllocator)

# renderdoc
include_directories(${PSET4_ROOT_DIR}/thirdparty/renderdoc)

# thirdparty
include_directories(${PSET4_ROOT_DIR}/thirdparty/glfw/include)
include_directories(${PSET4_ROOT_DIR}/thirdparty/spdlog)
include_directories(${PSET4_ROOT_DIR}/thirdparty/cxxopts)

# defines
add_definitions(-DPSET_DEBUG)
add_definitions(-DPSET_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${SOURCE_FILES_CC} ${SOURCE_FILES_C})

add_subdirectory(${CMAKE_SOURCE_DIR}/tools/SceModuleGenerator)

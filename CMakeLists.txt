cmake_minimum_required(VERSION 3.29)
project(cpp_cache)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64")

# Install Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# Google Test (Windows): Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# add src/ as CacheStrategies library
add_library(CacheStrategies INTERFACE)
target_include_directories(CacheStrategies INTERFACE ${PROJECT_SOURCE_DIR}/src)

add_executable(
        cache_tests
        tests/test_lru_cache.cpp
        tests/test_time_based_cache.cpp
        tests/test_lfu_cache.cpp
)

target_link_libraries(
        cache_tests
        CacheStrategies
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cache_tests)

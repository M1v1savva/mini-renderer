cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(TEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/tests_bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# --- Dependencies --- 
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

find_package(OpenSSL REQUIRED)

# --- Application ---

# - main -
add_executable(
  mini-renderer
  src/main.cpp
  src/graphics.cpp
	src/model.cpp
)
target_include_directories(mini-renderer PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(mini-renderer PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

# - model overview - 
add_executable(
  model-overview
  src/model.cpp
)
target_include_directories(mini-renderer PRIVATE ${CMAKE_SOURCE_DIR}/include)

# --- Tests ---
enable_testing()

function(test_math name sources)
    add_executable(${name} ${sources})
    target_include_directories(${name} PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/tests
        ${CMAKE_SOURCE_DIR}
    )
    target_link_libraries(${name} PRIVATE GTest::gtest GTest::gtest_main)
    add_test(NAME ${name} COMMAND ${name})
    set_tests_properties(${name} PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)
endfunction()

# vec
test_math(test_vec tests/linal/test_vec.cpp)

# matrix
test_math(test_matrix tests/linal/test_matrix.cpp)

# model
add_executable(test_model tests/test_model.cpp src/model.cpp)
target_include_directories(test_model PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/tests
    ${CMAKE_SOURCE_DIR}
)
target_link_libraries(test_model PRIVATE GTest::gtest GTest::gtest_main OpenSSL::SSL OpenSSL::Crypto)
add_test(NAME test_model COMMAND test_model)
set_tests_properties(test_model PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)
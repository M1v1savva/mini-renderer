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

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

# --- Application ---

# - main -
add_executable(
  mini-renderer
  src/main.cpp
  src/model.cpp
  src/graphics.cpp
  src/geometrics.cpp
  src/rasterizer_factory.cpp
  
  src/rasterizers/fill.cpp
  src/rasterizers/binary.cpp
  src/rasterizers/binary_gouraud.cpp
  src/rasterizers/binary_phong.cpp
  src/rasterizers/texture.cpp
  src/rasterizers/texture_gouraud.cpp
  src/rasterizers/texture_phong.cpp
)
target_include_directories(mini-renderer PRIVATE 
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}
)
target_link_libraries(mini-renderer PRIVATE nlohmann_json::nlohmann_json)

# - model overview - 
add_executable(
  model-overview
  src/model_overview.cpp
  src/model.cpp
)
target_include_directories(model-overview PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
)

# --- Tests ---
enable_testing()

function(add_my_tests name sources)
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
add_my_tests(test_vec tests/linal/test_vec.cpp)

# matrix
add_my_tests(test_matrix tests/linal/test_matrix.cpp)

# model
add_my_tests(test_model tests/test_model.cpp)

# graphics
add_executable(
  test_graphics 
  src/graphics.cpp 
  src/geometrics.cpp 
  src/rasterizer_factory.cpp 
  src/model.cpp
  tests/test_graphics.cpp
  src/rasterizers/fill.cpp
  src/rasterizers/binary.cpp
  src/rasterizers/binary_gouraud.cpp
  src/rasterizers/binary_phong.cpp
  src/rasterizers/texture.cpp
  src/rasterizers/texture_gouraud.cpp
  src/rasterizers/texture_phong.cpp
)
target_include_directories(test_graphics PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/tests
    ${CMAKE_SOURCE_DIR}
)
target_link_libraries(test_graphics PRIVATE GTest::gtest GTest::gtest_main)
add_test(NAME test_graphics COMMAND test_graphics)
set_tests_properties(test_graphics PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)

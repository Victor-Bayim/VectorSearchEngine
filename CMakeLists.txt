cmake_minimum_required(VERSION 3.15)
project(VectorSearchEngine VERSION 0.1 LANGUAGES CXX)

# 设置 C++ 标准（推荐使用 C++17）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

option(BUILD_TESTS "Build unit tests" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 添加公共头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# ----------------------------
# 创建库 target：VectorSearchEngineLib
# ----------------------------
file(GLOB_RECURSE LIB_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM LIB_SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp")
add_library(VectorSearchEngineLib ${LIB_SOURCES})
target_include_directories(VectorSearchEngineLib PUBLIC ${PROJECT_SOURCE_DIR}/include)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found. Linking OpenMP libraries.")
    target_link_libraries(VectorSearchEngineLib PUBLIC OpenMP::OpenMP_CXX)
endif()

# ----------------------------
# 创建主可执行文件 target：VectorSearchEngine (入口 main.cpp)
# ----------------------------
add_executable(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/src/main.cpp")
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE VectorSearchEngineLib)

# ----------------------------
# 创建测试目标（如果启用了 BUILD_TESTS 选项）
# ----------------------------
if(BUILD_TESTS)
    enable_testing()
    file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/tests/*.cpp")
    add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
    target_include_directories(${PROJECT_NAME}_tests PUBLIC ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(${PROJECT_NAME}_tests PRIVATE VectorSearchEngineLib)
    add_test(NAME RunUnitTests COMMAND ${PROJECT_NAME}_tests)
endif()

# 安装目标（可选）
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(SkipListProject VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include_directories(${CMAKE_SOURCE_DIR})

# 添加googletest库
add_subdirectory(third_party/googletest)

# 添加util目录中的文件作为库
add_library(arena util/arena.cpp)

# 将src目录下的所有源文件添加到主项目
file(GLOB SOURCES "src/*")
file(GLOB UTIL "util/*.h")

# 添加可执行文件
add_executable(SkipListTest tests/SkipListTests.cpp ${SOURCES} ${UTIL})
add_executable(MemTableTest tests/MemTableTests.cpp ${SOURCES} ${UTIL})

# 链接arena库和gtest库到可执行文件
target_link_libraries(SkipListTest arena gtest_main)
target_link_libraries(MemTableTest arena gtest_main)

# 启用测试
enable_testing()

# 添加一个简单的测试来运行
add_test(
    NAME SkipListTest
    COMMAND SkipListTest
)

add_test(NAME memtable_test COMMAND memtable_test)

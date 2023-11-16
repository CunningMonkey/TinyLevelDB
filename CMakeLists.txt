cmake_minimum_required(VERSION 3.10)

# Set the project name
project (TinyLevelDB)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/arena.cpp
    src/memtable.cpp
    src/table_builder.cpp
    )
    
include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(third_party/googletest)


add_executable(SkipListTest tests/skiplist_test.cpp ${SOURCES})
add_executable(MemTableTest tests/memtable_test.cpp ${SOURCES})


target_link_libraries(SkipListTest gtest_main)
target_link_libraries(MemTableTest gtest_main)

# 启用测试
enable_testing()

# 添加一个简单的测试来运行
add_test(
    NAME SkipListTest
    COMMAND SkipListTest
)

add_test(
    NAME MemTableTest
    COMMAND MemTableTest
)

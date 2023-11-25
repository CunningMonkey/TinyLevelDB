cmake_minimum_required(VERSION 3.10)

project (TinyLevelDB)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/arena.cpp
    src/memtable.cpp
    src/table_builder.cpp
    src/block_builder.cpp
    )
    
include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(third_party/googletest)


add_executable(SkipListTest tests/skiplist_test.cpp ${SOURCES})
add_executable(MemTableTest tests/memtable_test.cpp ${SOURCES})
add_executable(BlockBuilderTest tests/block_builder_test.cpp ${SOURCES})


target_link_libraries(SkipListTest gtest_main)
target_link_libraries(MemTableTest gtest_main)
target_link_libraries(BlockBuilderTest gtest_main)

enable_testing()

add_test(
    NAME SkipListTest
    COMMAND SkipListTest
)

add_test(
    NAME MemTableTest
    COMMAND MemTableTest
)

add_test(
    NAME BlockBuilderTest
    COMMAND BlockBuilderTest
)
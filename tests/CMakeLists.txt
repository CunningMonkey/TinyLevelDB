# 为GoogleTest设置路径，你可能需要根据实际路径进行修改
set(GTEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/googletest)

# 添加GoogleTest的子目录，使其可以在本项目中构建
add_subdirectory(${GTEST_ROOT} googletest)

# 添加测试可执行文件
add_executable(lock_free_tests LockFreeSkipListTests.cpp)
add_executable(tests SkipListTests.cpp)

# 链接测试可执行文件到我们的库和GoogleTest
target_link_libraries(tests skiplist gtest_main)
target_link_libraries(lock_free_tests skiplist gtest_main)

# 添加测试
enable_testing()
add_test(NAME tests COMMAND tests)

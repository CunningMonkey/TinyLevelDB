cmake_minimum_required(VERSION 3.10)

project(LockFreeSkipList)

# 将C++标准设置为C++11，因为你的代码中使用了C++11的功能
set(CMAKE_CXX_STANDARD 14)

# 加入src和tests目录
add_subdirectory(src)
add_subdirectory(tests)
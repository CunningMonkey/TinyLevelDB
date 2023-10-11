add_library(skiplist INTERFACE)
add_library(lockfreeskiplist INTERFACE)  # 这一行是新添加的

# 这一行适用于两个库，因为它们都在同一个目录中
target_include_directories(skiplist INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(lockfreeskiplist INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})  # 这一行是新添加的

# 创建一个静态库
add_library(lockfreeskiplist INTERFACE)

# 设定其包含的目录，这样其他目标(例如测试)可以找到头文件
target_include_directories(lockfreeskiplist INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

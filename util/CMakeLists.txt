# 添加库，名为util_lib，基于arena.cpp创建
add_library(util_lib arena.cpp)

# 如果其他目录需要访问此库的头文件，确保它们可以找到这些头文件
target_include_directories(util_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
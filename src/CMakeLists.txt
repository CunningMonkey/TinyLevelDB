add_library(skiplist INTERFACE)
target_sources(skiplist INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/SkipList.h
    ${CMAKE_CURRENT_SOURCE_DIR}/LockFreeSkipList.h
    util_lib
)

set(LLVM_LINK_COMPONENTS support)
if(NOT DEFINED CLANG_TIDY_TOPLEVEL)
message(FATAL_ERROR "CLANG_TIDY_TOPLEVEL IS NOT SET. EXITING.")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule1aCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule1aCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule1bCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule1bCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule1cCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule1cCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule1dCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule1dCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule2aCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule2aCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule2bCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule2bCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule3aCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule3aCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule3bCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule3bCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule3cCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule3cCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule3dCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule3dCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule3eCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule3eCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule3fCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule3fCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule4aCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule4aCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule4bCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule4bCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule4cCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule4cCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule5aCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule5aCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule5bCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule5bCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule5cCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule5cCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule5dCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule5dCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule5eCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule5eCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule6aCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule6aCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule7aCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule7aCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule8aCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule8aCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule8bCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule8bCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule8cCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule8cCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule8dCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule8dCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule8eCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule8eCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule8fCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule8fCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule8gCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule8gCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule9aCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule9aCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule9bCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule9bCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule9cCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule9cCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule9dCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule9dCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule9eCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule9eCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule10aCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule10aCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule11aCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule11aCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule11bCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule11bCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule11cCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule11cCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule11dCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule11dCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule11eCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule11eCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule12aCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule12aCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule12bCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule12bCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Rule12cCheck.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Rule12cCheck.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/EastwoodTidyModule.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/EastwoodTidyModule.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/EastwoodTidyUtil.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/EastwoodTidyUtil.h)

message(STATUS "INCLUDING CURRENT SOURCE DIR ${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(
    BEFORE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

add_clang_library(clangTidyEastwoodModule
    src/EastwoodTidyUtil.cpp
    src/EastwoodTidyModule.cpp
    src/Rule10aCheck.cpp
    src/Rule11aCheck.cpp
    src/Rule11bCheck.cpp
    src/Rule11cCheck.cpp
    src/Rule11dCheck.cpp
    src/Rule11eCheck.cpp
    src/Rule12aCheck.cpp
    src/Rule12bCheck.cpp
    src/Rule12cCheck.cpp
    src/Rule1aCheck.cpp
    src/Rule1bCheck.cpp
    src/Rule1cCheck.cpp
    src/Rule1dCheck.cpp
    src/Rule2aCheck.cpp
    src/Rule2bCheck.cpp
    src/Rule3aCheck.cpp
    src/Rule3bCheck.cpp
    src/Rule3cCheck.cpp
    src/Rule3dCheck.cpp
    src/Rule3eCheck.cpp
    src/Rule3fCheck.cpp
    src/Rule4aCheck.cpp
    src/Rule4bCheck.cpp
    src/Rule4cCheck.cpp
    src/Rule5aCheck.cpp
    src/Rule5bCheck.cpp
    src/Rule5cCheck.cpp
    src/Rule5dCheck.cpp
    src/Rule5eCheck.cpp
    src/Rule6aCheck.cpp
    src/Rule7aCheck.cpp
    src/Rule8aCheck.cpp
    src/Rule8bCheck.cpp
    src/Rule8cCheck.cpp
    src/Rule8dCheck.cpp
    src/Rule8eCheck.cpp
    src/Rule8fCheck.cpp
    src/Rule8gCheck.cpp
    src/Rule9aCheck.cpp
    src/Rule9bCheck.cpp
    src/Rule9cCheck.cpp
    src/Rule9dCheck.cpp
    src/Rule9eCheck.cpp

    LINK_LIBS
    clangAnalysis
    clangAST
    clangASTMatchers
    clangBasic
    clangLex
    clangSerialization
    clangTidy
    clangTidyUtils
    clangTooling
)

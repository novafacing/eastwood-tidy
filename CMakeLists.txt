set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidyEastwoodModule

    src/EastwoodTidyModule.cpp
    src/Rule1aCheck.cpp
    src/Rule1bCheck.cpp
    src/Rule1cCheck.cpp
    src/Rule1dCheck.cpp
    src/Rule4aCheck.cpp
    src/Rule4cCheck.cpp
    src/Rule11eCheck.cpp

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

include_directories(
    BEFORE
        include/
)

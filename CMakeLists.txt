set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidyEastwoodModule

    EastwoodTidyModule.cpp
    Rule1aCheck.cpp
    Rule1cCheck.cpp
    Rule1dCheck.cpp
    Rule4aCheck.cpp
    Rule4cCheck.cpp
    Rule11eCheck.cpp

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

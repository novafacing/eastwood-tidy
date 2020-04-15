set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidyEastwoodModule
  EastwoodTidyModule.cpp
  Rule11eCheck.cpp
  Rule1cCheck.cpp
  Rule1dCheck.cpp
  Rule1aCheck.cpp
  Rule4aCheck.cpp
  Rule4cCheck.cpp

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

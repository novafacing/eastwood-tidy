set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidyEastwoodModule
  EastwoodTidyModule.cpp
  Rule11eCheck.cpp
  Rule1DCheck.cpp
  Rule1aCheck.cpp
  Rule5dCheck.cpp

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

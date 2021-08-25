set(LLVM_LINK_COMPONENTS
  Support
  )

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/clang-tidy-config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/clang-tidy-config.h)
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

add_clang_library(clangTidy
  ClangTidy.cpp
  ClangTidyCheck.cpp
  ClangTidyModule.cpp
  ClangTidyDiagnosticConsumer.cpp
  ClangTidyOptions.cpp
  ClangTidyProfiling.cpp
  ExpandModularHeadersPPCallbacks.cpp
  GlobList.cpp

  DEPENDS
  ClangSACheckers

  LINK_LIBS
  clangAnalysis
  clangAST
  clangASTMatchers
  clangBasic
  clangFormat
  clangFrontend
  clangLex
  clangRewrite
  clangSema
  clangSerialization
  clangTooling
  clangToolingCore
  )

if(CLANG_TIDY_ENABLE_STATIC_ANALYZER)
  clang_target_link_libraries(clangTidy
    PRIVATE
    clangStaticAnalyzerCore
    clangStaticAnalyzerFrontend
  )
endif()
if(CLANG_TIDY_ENABLE_STATIC_ANALYZER)
  add_subdirectory(mpi)
endif()

# Checks.
# If you add a check, also add it to ClangTidyForceLinker.h in this directory.
set(CLANG_TIDY_TOPLEVEL ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(eastwood-tidy)
set(ALL_CLANG_TIDY_CHECKS
  clangTidyEastwoodModule
  )
if(CLANG_TIDY_ENABLE_STATIC_ANALYZER)
  list(APPEND ALL_CLANG_TIDY_CHECKS clangTidyMPIModule)
endif()
set(ALL_CLANG_TIDY_CHECKS ${ALL_CLANG_TIDY_CHECKS} PARENT_SCOPE)

# Other subtargets. These may reference ALL_CLANG_TIDY_CHECKS
# and must be below its definition.
add_subdirectory(plugin)
add_subdirectory(tool)
add_subdirectory(utils)

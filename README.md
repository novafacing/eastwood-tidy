# eastwood-tidy
Port of Eastwood to Clang-Tidy Checks

Setup

1. Get the llvm github repo
    - git clone https://github.com/llvm/llvm-project.git
    
2. Add eastwood-tidy to llvm-project/clang-tools-extra/clang-tidy/eastwood
    - git clone https://github.com/Connor-McMillin/eastwood-tidy.git eastwood
    
Make sure that the eastwood-tidy repo is named eastwood and is in llvm-project/clang-tools-extra/clang-tidy/ since the rest of the setup depends on this structure and naming

3. Copy files from eastwood/setup to the proper places in llvm-project/clang-tools-extra/clang-tidy/
    - cp eastwood/setup/CMakeLists.txt llvm-project/clang-tools-extra/clang-tidy/
    - cp eastwood/setup/ClangTidyForceLinker.h llvm-project/clang-tools-extra/clang-tidy/
    
4. Use CMake + Make to build the new clang-tidy
    - cd /llvm-project/llvm/
    - mkdir build
    - cd build
    - cmake -DLLVM_ENABLE_PROJECTS='clang;clang-tools-extra' ..
    - make -j NN clang-tidy
    
And now you should have the new clang-tidy with all the currently developed eastwood-tidy checks in it in llvm-project/llvm/build/bin.

To run the clang-tidy linter with only our rules you will need a few things:
  - test.c : a C file to check
  - build_commands.json : A json file of how to compile every C file. Put it in current directory and clang-tidy will find it

Command to run eastwood checks:
  clang-tidy -checks "-\*,eastwood\*" test.c

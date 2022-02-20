# Clang Tidy Module Tests

Running tests is handled using `pytest` instead of `gtest` or the 
[bespoke method](https://github.com/llvm/llvm-project/blob/main/clang-tools-extra/test/clang-tidy/check_clang_tidy.py)
provided by `LLVM`.

## File Tests


## Snippet Tests

Snippet tests allow specifying just a short code block like:

```
#define 10
```

that should trigger a clang tidy error. They are found in `testcases/snippets/`.
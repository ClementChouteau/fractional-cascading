# Fractional Cascading

- [ ] Cleanup `generation.hpp`.
- [ ] Make the code generic on key type with templates.
- [ ] Allow to choose between *copy the values given* or *store a pointer to them*.
- [ ] Allow to visit all values equals to the key (if a list contains `1, 2, 3, 3, 4` and we search for `3`).
- [ ] Find a mechanism more generic than the `onFound` callback.
- [ ] Setup GitHub CI pipeline for this project with build, clang-tidy, test, benchmark, coverage.

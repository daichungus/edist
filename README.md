# edist
A [Levenshtein edit distance](https://wikipedia.org/wiki/Levenshtein_distance)
calculator with multithreading and AVX2.
# Requirements
- x86 CPU with [AVX2](https://wikipedia.org/wiki/Advanced_Vector_Extensions)
- A Linux-based or POSIX-compliant operating system
- A C compiler, such as `gcc` or `clang` 
# Build instructions
1. Clone this repo and `cd edist/`
2. Run `make` (to use clang, add `CC=clang`)
# Usage
To compare a pair of strings, each consisting of randomly generated `n`
characters:

`./edist -n <string_length>`

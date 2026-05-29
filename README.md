# edist
A [Levenshtein edit distance](https://wikipedia.org/wiki/Levenshtein_distance)
calculator with multithreading and AVX2.
# Requirements
- x86 CPU with [AVX2](https://wikipedia.org/wiki/Advanced_Vector_Extensions)
- A Linux-based or POSIX-compliant operating system
- A C compiler, such as `gcc` or `clang` 
# Build instructions
1. Clone this repo and `cd edist/`
2. Run `make` (to use clang, run `CC=clang make`)
# Usage
To compare a pair of strings, each consisting of randomly generated `n`
characters:
```
./edist -n <string_length>
```
By default, number of allocated threads will be automatically calculated
depending on the length of the strings and the CPU's cache size.

The user can specify a specific number of threads to be allocated with `-t`
```
./edist -n <string_length> -t <thread_count>
```
Note that the specified thread count must have a minimum value of `1`, and the
maximum thread count will depend on your CPU's maximum thread count. It is also
possible that there are no available threads.

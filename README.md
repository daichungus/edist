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
## Single test
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
## Automatic repeating test (ART)
A Bash shell script is included with this repo to automatically run the
`edist` program for a pair of randomly-generated strings consisting of
`len` characters about `n` times in a row.

The shell script will also automatically record the output as a log file
stored in the subdirectory `logs/`

To use the ART shell script, specify the number of tests you want to run,
along with the string length.
```
./run_tests.sh <num_tests> <string_length>
```
The default test without any arguments whatsoever is the equivalent of
```
./run_tests.sh 3 1000000
```
which is equivalent to running
```
./edist -n 1000000
```
three times in a row.

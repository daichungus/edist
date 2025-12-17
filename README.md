# edist
A Levenshtein edit distance calculator utilizing multithreading and AVX2.
# Requirements
- Currently, this only runs on systems based on the Linux kernel and x86 CPU's that support AVX2
- Right now, only strings of the exact same length are supported
# Build instructions
1. Clone this repo and `cd edist`
2. Run `make`
3. To run the program, `./edist`

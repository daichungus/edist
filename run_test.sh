#!/bin/bash
make clean
make

printf "START: " | tee -a log.txt
date | tee -a log.txt

./edist | tee -a log.txt

printf "END: " | tee -a log.txt
date | tee -a log.txt
echo | tee -a log.txt

make clean

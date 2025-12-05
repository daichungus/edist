#!/bin/bash
make clean
make

LOG_DIR="./logs"
mkdir -p "$LOG_DIR"

TIMESTAMP=$(date +"%Y-%m-%d_%H-%M-%S")
LOG_FILE="$LOG_DIR/log_$TIMESTAMP.txt"

printf "START: " | tee -a "$LOG_FILE"
date | tee -a "$LOG_FILE"
echo | tee -a "$LOG_FILE"

N="3"
for i in $(seq 1 $N)
do
	./edist | tee -a "$LOG_FILE"
done 

printf "END: " | tee -a "$LOG_FILE"
date | tee -a "$LOG_FILE"
echo | tee -a "$LOG_FILE"

make clean

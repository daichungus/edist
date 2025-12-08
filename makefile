CC_GCC := gcc
CC_CLANG := clang

CFLAGS := -O3 -pthread -mavx -mfma -march=native -Wall -Wextra -Werror

SRCS := main.c edit_distance.c test_edit_distance.c tile_calculator.c

OBJS_GCC := $(SRCS:.c=.gcc.o)
OBJS_CLANG := $(SRCS:.c=.clang.o)

TARGET_GCC := edist
TARGET_CLANG := edist-clang

all: $(TARGET_GCC) $(TARGET_CLANG)

$(TARGET_GCC): $(OBJS_GCC)
	$(CC_GCC) $(CFLAGS) -o $@ $^
	rm $(OBJS_GCC)

$(TARGET_CLANG): $(OBJS_CLANG)
	$(CC_CLANG) $(CFLAGS) -o $@ $^
	rm $(OBJS_CLANG)
	
%.gcc.o: %.c
	$(CC_GCC) $(CFLAGS) -c -o $@ $<

%.clang.o: %.c
	$(CC_CLANG) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET_GCC) $(TARGET_CLANG) *.o

.PHONY: all clean

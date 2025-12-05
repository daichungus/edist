CC_GCC := gcc

CFLAGS := -O3 -pthread -mavx -mfma -march=native -Wall -Wextra -Werror

SRCS := main.c edit_distance.c test_edit_distance.c

OBJS_GCC := $(SRCS:.c=.o)

TARGET_GCC := edist

all: $(TARGET_GCC)

$(TARGET_GCC): $(OBJS_GCC)
	$(CC_GCC) $(CFLAGS) -o $@ $^

%.gcc.o: %.c
	$(CC_GCC) $(CFLAGS) -c -o $@ $< 

clean:
	rm -f $(TARGET_GCC) *.o

.PHONY: all clean

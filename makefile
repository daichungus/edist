CC ?= gcc

CFLAGS := -O3 -pthread -mavx -mfma -march=native -Wall -Wextra -Werror -fsanitize=address -fno-omit-frame-pointer

SRCS := main.c edit_distance.c test_edit_distance.c tile_calculator.c

OBJS := $(SRCS:.c=.o)

TARGET := edist

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	rm $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean

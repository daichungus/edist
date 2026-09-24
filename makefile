CC ?= gcc

CFLAGS := -O3 -pthread -mavx -mavx2 -mfma -march=native -Wall -Wextra -Werror

SRCS := src/main.c src/edit_distance.c src/test_edit_distance.c src/tile_calculator.c

OBJS := $(SRCS:.c=.o)

TARGET := edist

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	rm $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)

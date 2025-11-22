CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
INCLUDES = -I../../core
SRCS = main.c focas_mock.c
OBJS = $(SRCS:.c=.o)
TARGET = fanuc-sim

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) ../../core/cnc_state.o
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) ../../core/cnc_state.o

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

../../core/cnc_state.o:
	$(CC) $(CFLAGS) -c ../../core/cnc_state.c -o ../../core/cnc_state.o

clean:
	rm -f $(OBJS) $(TARGET) ../../core/cnc_state.o

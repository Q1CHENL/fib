.PHONY: all clean

CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lm
OBJFILES = main.o fib_v0.o fib_v1.o mul.o cvt.o
TARGET = fib

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET)

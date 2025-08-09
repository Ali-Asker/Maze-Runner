CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra
TARGET = mazerunner
SRCS = mazegeneration.c utility.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
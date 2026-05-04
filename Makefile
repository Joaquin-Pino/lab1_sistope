CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm
TARGET = lab1

OBJS = lab1.o funciones.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

lab1.o: lab1.c funciones.h
	$(CC) $(CFLAGS) -c lab1.c

funciones.o: funciones.c funciones.h
	$(CC) $(CFLAGS) -c funciones.c

clean:
	rm -f $(OBJS) $(TARGET)
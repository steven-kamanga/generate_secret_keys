CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS =

all: generate

generate: main.c generator.c file_writer.c
    $(CC) $(CFLAGS) $(LDFLAGS) -o generate main.c generator.c file_writer.c

install:
    cp generate /usr/local/bin

clean:
    rm generate

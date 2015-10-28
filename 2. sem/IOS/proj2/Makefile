
DEPS=h2o.h
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread
OBJECT=h2o.o
CC=gcc

all: h2o .PHONY
.PHONY: clean

%.o:%.c $(DEPS)
	@$(CC) $(CFLAGS) -c -o $@ $<

h2o:$(OBJECT)
	@$(CC) $(CFLAGS) -o $@ $^

clean:
	@rm -f *.o
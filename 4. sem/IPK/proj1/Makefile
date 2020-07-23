
compile: webclient

CC=g++
OBJECT=webclient.o

%.o:%.cpp
	$(CC) -c -o $@ $<

webclient:$(OBJECT)
	$(CC) -o $@ $^

clean:
	rm -f *.o
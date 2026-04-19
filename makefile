CFLAGS=-Wall
LDFLAGS=-lcrypt

proj: main.o lib.o
	gcc $^ $(LDFLAGS) -o $@

%.o: %.c lib.h
	gcc $(CFLAGS) -c $<

clean:
	rm -f *.o proj


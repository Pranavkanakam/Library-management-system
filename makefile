CFLAGS=-Wall

proj: main.o lib.o
	gcc $^ -o $@

%.o: %.c lib.h
	gcc $(CFLAGS) -c $<

clean:
	rm -f *.o proj


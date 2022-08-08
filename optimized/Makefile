CC=gcc
OBJ = optimized.o
LIB = -fopenmp

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(LIB)

optimized: $(OBJ)
	gcc $(CFLAGS) -o $@ $^ $(LIB)

clean:
	rm *.o optimized	

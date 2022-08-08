CC=gcc
OBJ = parallel.o
LIB = -fopenmp

%.o: %.c $(DEPS) 
	$(CC) -c -o $@ $< $(LIB)

parallel: $(OBJ)
	gcc $(CFLAGS) -o $@ $^ $(LIB)

clean:
	rm *.o parallel	

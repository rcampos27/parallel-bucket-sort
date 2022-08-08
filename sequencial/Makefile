CC=gcc
OBJ = sequential.o
LIB = -fopenmp

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(LIB)

sequential: $(OBJ)
	gcc $(CFLAGS) -o $@ $^ $(LIB)

clean:
	rm *.o sequential	

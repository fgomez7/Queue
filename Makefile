CC = clang
CFLAGS = -Wall -Werror -Wextra -pedantic

Sources := queue.c
Objects := queue.o

queue.o: %.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< -pthread

clean:
	rm -rf $(Objects)


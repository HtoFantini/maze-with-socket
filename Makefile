BIN_DIR = bin

all:
	@mkdir -p $(BIN_DIR)
	gcc -Wall -c server.c -o server.o
	gcc -Wall -c client.c -o client.o
	gcc -Wall server.o -o $(BIN_DIR)/server
	gcc -Wall client.o -o $(BIN_DIR)/client

clean:
	rm -rf $(BIN_DIR) *.o

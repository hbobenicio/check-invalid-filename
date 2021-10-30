BIN = bin/check-invalid-filenames

.phony: distclean

$(BIN): src/main.c
	$(CC) -Wall -Wextra -pedantic $(CFLAGS) -o $(BIN) src/main.c

clean:
	$(RM) $(BIN)

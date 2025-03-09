CC = gcc
CFLAGS = -ggdb -O3 -fsanitize=address,undefined -Wall -Wextra

check: ./main.c ./trie.c ./read_sup.c ./read_med.c ./read_bat.c ./io.c ./dates.c ./utils.c
	$(CC) $(CFLAGS) -o check main.c trie.c read_sup.c ./read_med.c ./read_bat.c ./io.c ./dates.c ./utils.c

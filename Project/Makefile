CFLAGS = -g -Wall -Werror

all: Blackjack

Blackjack: gamehandler.o savehandler.o
	gcc $^ -g -o Blackjack

%.o: %.c blackjack.h
	gcc $< -c $(CFLAGS) -o $@

clean:
	rm -f *.o Blackjack

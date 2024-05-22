all: taquin board_nqueens.o board_knights.o board_taquin.o list.o

list.o: list.c list.h
	gcc -c list.c

#--------------------- QUEENS -------------------------

board_nqueens.o : board_queens.c board.h

nqueens: nqueens.c board_queens.c list.c board.h list.h item.h
	gcc -o nqueens nqueens.c board_queens.c list.c



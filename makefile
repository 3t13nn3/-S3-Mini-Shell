#Makefile Générique
#Projet MINI-SHELL
#Etienne PENAULT 17805598

OBJS	= main.o fonction.o
OUT	= mini-shell
CC	 = gcc
FLAGS	 = -O3 -c -Wall

all: $(OBJS)
	$(CC) $(OBJS) -o $(OUT)
main.o: main.c
	$(CC) $(FLAGS) main.c 
fonction.o: fonction.c
	$(CC) $(FLAGS) fonction.c
clean:
	rm -f $(OBJS) $(OUT)

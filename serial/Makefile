# compile and link code
main: main.o chromo.o group.o prisoner_dilemma.o
	gcc -O2 -o main main.o chromo.o group.o prisoner_dilemma.o

main.o: main.c chromo.h group.h
	gcc -c main.c

chromo.o: chromo.c chromo.h
	gcc -c chromo.c

group.o: group.c group.h
	gcc -c group.c

prisoner_dilemma.o: prisoner_dilemma.c prisoner_dilemma.h
	gcc -c prisoner_dilemma.c

# clean target
clean:
	rm -f main main.o chromo.o group.o prisoner_dilemma.o

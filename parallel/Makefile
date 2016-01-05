# compile and link code
main: main.o chromo.o group.o
	mpicc -o main main.o chromo.o group.o

main.o: main.c chromo.h group.h
	mpicc -c main.c

chromo.o: chromo.c chromo.h
	mpicc -c chromo.c

group.o: group.c group.h
	mpicc -c group.c

# clean target
clean:
	rm -f main main.o chromo.o group.o

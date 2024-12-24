.PHONY: clean
fundraiser: input.o group.o fundraiser.o
	gcc input.o group.o fundraiser.o -o fundraiser
fundraiser.o: fundraiser.c input.h group.h
	gcc -Wall -std=c99 -c fundraiser.c
input.o: input.c input.h
	gcc -Wall -std=c99 -c input.c
group.o: group.c group.h input.h
	gcc -Wall -std=c99 -c group.c
clean:
	rm -f *.o fundraiser